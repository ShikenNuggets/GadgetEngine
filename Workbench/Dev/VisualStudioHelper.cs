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
        private static readonly string _programId = "VisualStudio.DTE.17.0";

        [DllImport("ole32.dll")]
        private static extern int CreateBindCtx(uint reserved, out IBindCtx ppbc);

        [DllImport("ole32.dll")]
        private static extern int GetRunningObjectTable(uint reserved, out IRunningObjectTable pprot);

        public static void OpenVisualStudio(string solutionPath)
        {
            Debug.Assert(solutionPath != null);

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
                _vsInstance.ExecuteCommand("File.SaveAll");
                _vsInstance.Solution.Close();
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
    }
}