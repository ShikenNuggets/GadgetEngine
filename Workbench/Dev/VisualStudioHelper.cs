using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Threading.Tasks;

namespace Workbench
{
    public static class VisualStudioHelper
    {
        public const string SolutionExtension = ".sln";

        private static EnvDTE._DTE? _vsInstance = null;
        private static readonly string _programId = "VisualStudio.DTE.17.0"; //VS2022 internal version number

        [DllImport("ole32.dll")]
        private static extern int CreateBindCtx(uint reserved, out IBindCtx ppbc);

        [DllImport("ole32.dll")]
        private static extern int GetRunningObjectTable(uint reserved, out IRunningObjectTable pprot);

        public static bool LastBuildSucceeded { get; private set; } = true;
        public static bool BuildDone { get; private set; } = true;

        public static void OpenVisualStudio(string solutionPath)
        {
            Debug.Assert(solutionPath != null);

            Logger.Log(MessageType.Info, "Checking if Visual Studio is open...");

            IRunningObjectTable? rot = null;
            IEnumMoniker? monikerTable = null;
            IBindCtx? bindContext = null;

            try
            {
                if (_vsInstance == null)
                {
                    var hResult = GetRunningObjectTable(0, out rot);
                    if (hResult < 0 || rot == null) { throw new COMException($"GetRunningObjectTable() returned HRESULT: {hResult:X8}"); }

                    rot.EnumRunning(out monikerTable);
                    monikerTable.Reset();

                    hResult = CreateBindCtx(0, out bindContext);
                    if (hResult < 0 || bindContext == null) { throw new COMException($"CreateBindCtx() returned HRESULT: {hResult:X8}"); }

                    IMoniker[] currentMoniker = new IMoniker[1];
                    bool isOpen = false;
                    while (!isOpen && monikerTable.Next(1, currentMoniker, IntPtr.Zero) == 0)
                    {
                        string name = string.Empty;
                        currentMoniker[0]?.GetDisplayName(bindContext, null, out name);
                        if (name.Contains(_programId))
                        {
                            hResult = rot.GetObject(currentMoniker[0], out object obj);
                            if (hResult < 0 || obj == null) { throw new COMException($"IRunningObjectTable.GetObject() returned HRESULT: {hResult:X8}"); }

                            if (obj is not EnvDTE._DTE dte)
                            {
                                //Something went wrong... not sure what, though
                                Logger.Log(MessageType.Warning, $"Program instance with program ID '{_programId}' could not be converted to DTE interface!");
                                continue;
                            }

                            var solutionName = dte.Solution.FullName;
                            if (solutionName == solutionPath)
                            {
                                Logger.Log(MessageType.Verbose, "Found Visual Studio instance that was already open.");
                                _vsInstance = dte;
                                isOpen = true;
                            }
                        }
                    }

                    if (_vsInstance == null)
                    {
                        Type? visualStudioType = Type.GetTypeFromProgID(_programId, true);
                        if (visualStudioType != null)
                        {
                            Logger.Log(MessageType.Info, "Launching Visual Studio...");
                            _vsInstance = Activator.CreateInstance(visualStudioType) as EnvDTE._DTE;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, "Failed to open Visual Studio");
            }
            finally
            {
                if (bindContext != null)
                {
                    Marshal.ReleaseComObject(bindContext);
                }

                if (monikerTable != null)
                {
                    Marshal.ReleaseComObject(monikerTable);
                }

                if (rot != null)
                {
                    Marshal.ReleaseComObject(rot);
                }
            }
        }

        public static void CloseVisualStudio()
        {
            if (_vsInstance?.Solution.IsOpen == true)
            {
                Logger.Log(MessageType.Info, "Saving all files in Visual Studio...");
                _vsInstance.ExecuteCommand("File.SaveAll");
                if (IsBusy())
                {
                    Logger.Log(MessageType.Info, "Stopping Visual Studio debugger...");
                    _vsInstance.Debugger.Stop();
                }

                Logger.Log(MessageType.Info, "Closing Visual Studio...");
                _vsInstance.Solution.Close();
            }
            else
            {
                Logger.Log(MessageType.Verbose, "Tried to close Visual Studio when it isn't open.");
            }
            _vsInstance?.Quit();
        }

        public static bool AddFilesToSolution(string solutionPath, string projectName, string[] files)
        {
            Debug.Assert(!string.IsNullOrWhiteSpace(solutionPath));
            Debug.Assert(!string.IsNullOrWhiteSpace(projectName));
            Debug.Assert(files != null && files.Length > 0);

            OpenVisualStudio(solutionPath); //Try to open Visual Studio if it's not already open

            try
            {
                if(_vsInstance == null)
                {
                    Debug.Write("Visual Studio could not be opened!");
                    Logger.Log(MessageType.Error, "Visual Studio could not be opened!");
                    return false;
                }

                if (_vsInstance.Solution.IsOpen == false)
                {
                    Logger.Log(MessageType.Info, "Opening solution in Visual Studio...");
                    _vsInstance.Solution.Open(solutionPath);
                }
                else
                {
                    _vsInstance.ExecuteCommand("File.SaveAll"); //Save All in case something goes wrong while adding the files
                }

                foreach(EnvDTE.Project project in _vsInstance.Solution.Projects)
                {
                    if (project.UniqueName.Contains(projectName))
                    {
                        foreach (string file in files)
                        {
                            Logger.Log(MessageType.Info, $"Adding {file} to Visual Studio solution...");
                            project.ProjectItems.AddFromFile(file);
                        }
                    }

                    var cpp = files.FirstOrDefault(x => Path.GetExtension(x) == ".cpp");
                    var h = files.FirstOrDefault(x => Path.GetExtension(x) == ".h");
                    if (!string.IsNullOrWhiteSpace(cpp))
                    {
                        _vsInstance.ItemOperations.OpenFile(cpp).Visible = true;
                    }

                    if (!string.IsNullOrWhiteSpace(h))
                    {
                        _vsInstance.ItemOperations.OpenFile(h).Visible = true;
                    }

                    _vsInstance.MainWindow.Activate();
                    _vsInstance.MainWindow.Visible = true;
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Debug.Write("Failed to add files to VS project!");
                Logger.Log(MessageType.Error, "Failed to add files to VS project!");
                return false;
            }

            return true;
        }

        public static bool IsBusy()
        {
            bool result = false;

            for (int i = 0; i < 3 && !result; i++)
            {
                try
                {
                    result = _vsInstance != null && (_vsInstance.Debugger.CurrentProgram != null || _vsInstance.Debugger.CurrentMode == EnvDTE.dbgDebugMode.dbgRunMode);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                    if (!result)
                    {
                        System.Threading.Thread.Sleep(1000);
                    }
                }
            }

            return result;
        }

        public static void BuildSolution(ProjectVM project, string config, bool showWindow = true)
        {
            if (IsBusy())
            {
                Logger.Log(MessageType.Info, "Visual Studio is currently running a process.");
                return;
            }

            OpenVisualStudio(project.SolutionPath);
            LastBuildSucceeded = BuildDone = false;

            if (_vsInstance == null)
            {
                Logger.Log(MessageType.Error, "Could not start a build because Visual Studio could not be opened.");
                return; //Couldn't open Visual Studio, can't start a build
            }

            for (int i = 0; i < 3; i++)
            {
                try
                {
                    if (!_vsInstance.Solution.IsOpen)
                    {
                        _vsInstance.Solution.Open(project.SolutionPath);
                    }
                    _vsInstance.MainWindow.Visible = showWindow;

                    _vsInstance.Events.BuildEvents.OnBuildProjConfigBegin += OnBuildSolutionBegin;
                    _vsInstance.Events.BuildEvents.OnBuildProjConfigDone += OnBuildSolutionDone;

                    _vsInstance.Solution.SolutionBuild.SolutionConfigurations.Item(config).Activate();
                    _vsInstance.ExecuteCommand("Build.BuildSolution");
                    break;
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                    Logger.Log(MessageType.Error, $"Attempt {i} to build {project.Name} failed!");
                }
            }
        }

        private static void OnBuildSolutionBegin(string project, string projectConfig, string platform, string solutionConfig)
        {
            Logger.Log(MessageType.Info, $"Building {project}, {projectConfig}, {platform}, {solutionConfig}");
        }

        private static void OnBuildSolutionDone(string project, string projectConfig, string platform, string solutionConfig, bool success)
        {
            if (BuildDone)
            {
                return;
            }

            if (success)
            {
                Logger.Log(MessageType.Info, $"Build succeeded.");
            }
            else
            {
                Logger.Log(MessageType.Error, $"Build failed!");
            }

            BuildDone = true;
            LastBuildSucceeded = success;
        }
    }
}