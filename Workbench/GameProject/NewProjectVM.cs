using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace Workbench
{
    [DataContract]
    public class ProjectTemplate
    {
        [DataMember] public required string ProjectType { get; set; }
        [DataMember] public required string ProjectFile { get; set; }
        [DataMember] public required List<string> Folders { get; set; }

        public string? RootPath { get; set; }
        public byte[]? Icon { get; set; }
        public byte[]? Screenshot { get; set; }
        public string? IconFilePath { get; set; }
        public string? ScreenshotFilePath { get; set; }
        public string? ProjectFilePath { get; set; }
    }

    class NewProjectVM : BaseViewModel
    {
        private readonly string _templatePath = @"ProjectTemplates\";

        private string _projectName = "NewProject";
        private string _path = $@"{Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)}\GadgetWorkbench\";

        public string ProjectName
        {
            get => _projectName;

            set
            {
                if (_projectName != value)
                {
                    _projectName = value;
                    ValidateProjectPath();
                    OnPropertyChanged(nameof(ProjectName));
                }
            }
        }

        public string ProjectPath
        {
            get => _path;

            set
            {
                if (_path != value)
                {
                    _path = value;
                    ValidateProjectPath();
                    OnPropertyChanged(nameof(Path));
                }
            }
        }

        private bool _isValid = true;
        public bool IsValid
        {
            get => _isValid;
            set
            {
                if (_isValid != value)
                {
                    _isValid = value;
                    OnPropertyChanged(nameof(IsValid));
                }
            }
        }

        private string _errorMessage = string.Empty;
        public string ErrorMessage
        {
            get => _errorMessage;
            set
            {
                if (_errorMessage != value)
                {
                    _errorMessage = value;
                    OnPropertyChanged(nameof(ErrorMessage));
                }
            }
        }

        private ObservableCollection<ProjectTemplate> _projectTemplates = new();
        public ReadOnlyObservableCollection<ProjectTemplate>? ProjectTemplates { get; }

        private bool ValidateProjectPath()
        {
            var path = ProjectPath;
            if (!Path.EndsInDirectorySeparator(path))
            {
                path += @"\";
            }
            path += $@"{ProjectName}\";

            IsValid = false;
            if (string.IsNullOrWhiteSpace(ProjectName.Trim()))
            {
                ErrorMessage = "Error: Please enter a project name.";
            }
            else if (ProjectName.IndexOfAny(Path.GetInvalidFileNameChars()) != -1)
            {
                ErrorMessage = "Error: Invalid character(s) used in project name [" + ProjectName.ElementAt(ProjectName.IndexOfAny(Path.GetInvalidFileNameChars())) + "].";
            }
            else if (string.IsNullOrWhiteSpace(ProjectPath.Trim()))
            {
                ErrorMessage = "Error: Please enter a project path.";
            }
            else if (ProjectPath.IndexOfAny(Path.GetInvalidPathChars()) != -1)
            {
                ErrorMessage = "Error: Invalid character(s) used in project path [" + ProjectName.ElementAt(ProjectName.IndexOfAny(Path.GetInvalidPathChars())) + "].";
            }
            else if (Directory.Exists(path) && Directory.EnumerateFileSystemEntries(path).Any())
            {
                ErrorMessage = "Error: Project path already exists and is not empty.";
            }
            else
            {
                ErrorMessage = string.Empty;
                IsValid = true;
            }

            return IsValid;
        }

        public string CreateProject(ProjectTemplate template)
        {
            ValidateProjectPath();
            if (!IsValid)
            {
                return string.Empty;
            }

            if (!Path.EndsInDirectorySeparator(ProjectPath))
            {
                ProjectPath += @"\";
            }
            var finalPath = $@"{ProjectPath}{ProjectName}\";

            try
            {
                Debug.Assert(finalPath != null);

                if (Directory.Exists(finalPath))
                {
                    Directory.CreateDirectory(finalPath);
                }

                foreach (var folder in template.Folders)
                {
                    Debug.Assert(!string.IsNullOrWhiteSpace(folder));

                    string folderPath = Path.GetFullPath(Path.Combine(finalPath, folder));
                    Directory.CreateDirectory(folderPath);

                    var dirInfo = new DirectoryInfo(folderPath);
                    if (folder[0] == '.')
                    {
                        dirInfo.Attributes |= FileAttributes.Hidden; //Folders that start with a . are implied to be hidden folders
                    }
                }

                string? templateIconFilePath = template.IconFilePath;
                string? templateScreenshotFilePath = template.ScreenshotFilePath;
                string? iconFileName = Path.GetFileName(template.IconFilePath);
                string? screenshotFileName = Path.GetFileName(template.ScreenshotFilePath);

                List<string?> filesToIgnore = new()
                {
                    "template.xml",
                    template.ProjectFile,
                    iconFileName,
                    screenshotFileName,
                    "MSVCSolution",
                    "MSVCProject"
                };

                Debug.Assert(!string.IsNullOrWhiteSpace(template.ProjectFilePath));
                if (string.IsNullOrWhiteSpace(template.ProjectFilePath) )
                {
                    throw new NullReferenceException(nameof(template.ProjectFilePath) + " was null");
                }

                var newDiretory = new FileInfo(template.ProjectFilePath).Directory;
                Debug.Assert(newDiretory != null);
                if (newDiretory == null )
                {
                    throw new NullReferenceException(nameof(newDiretory) + " was null");
                }

                //Copy all files from the template to the new directory
                Utils.CopyDirectory(newDiretory.FullName, finalPath, true, filesToIgnore);

                //Copy the project file, with the new name and new information
                var projectXml = File.ReadAllText(template.ProjectFilePath);
                projectXml = string.Format(projectXml, ProjectName, finalPath);
                var projectPath = ProjectVM.GetFullPath(finalPath, ProjectName);
                File.WriteAllText(projectPath, projectXml);

                CreateMSVCSolution(template, finalPath);

                //Copy the Icon and Screenshot to the temp folder
                if(template.IconFilePath != null)
                {
                    File.Copy(template.IconFilePath, finalPath + ProjectVM.TempDir + Path.GetFileName(template.IconFilePath));
                }

                if (template.ScreenshotFilePath != null)
                {
                    File.Copy(template.ScreenshotFilePath, finalPath + ProjectVM.TempDir + Path.GetFileName(template.ScreenshotFilePath));
                }

                return finalPath;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, $"Failed to create new {template.ProjectType} project!");
                throw;
            }
        }

        private void CreateMSVCSolution(ProjectTemplate template, string projectPath)
        {
            Debug.Assert(template != null && template.RootPath != null);

            string vcSolutionPath = Path.Combine(template.RootPath, "MSVCSolution");
            string vcProjectPath = Path.Combine(template.RootPath, "MSVCProject");

            Debug.Assert(File.Exists(vcSolutionPath));
            Debug.Assert(File.Exists(vcProjectPath));

            var gadgetIncludePath = Path.Combine(MainWindow.GadgetEnginePath, @"include\GadgetEngine\");
            var gadgetLibraryPath = Path.Combine(MainWindow.GadgetEnginePath, @"lib\");

            List<string> gadgetIncludes = new()
            {
                gadgetIncludePath,
                Path.Combine(MainWindow.GadgetEnginePath, @"include\Assimp\"),
                Path.Combine(MainWindow.GadgetEnginePath, @"include\bullet3\"),
                Path.Combine(MainWindow.GadgetEnginePath, @"include\common\"),
                Path.Combine(MainWindow.GadgetEnginePath, @"include\freetype\"),
                Path.Combine(MainWindow.GadgetEnginePath, @"include\Glad\"),
                Path.Combine(MainWindow.GadgetEnginePath, @"include\SDL2\")
            };

            Debug.Assert(Directory.Exists(gadgetIncludePath));
            Debug.Assert(Directory.Exists(gadgetLibraryPath));

            var projectName = ProjectName;
            var projectGUID = "{" + Guid.NewGuid().ToString().ToUpper() + "}";
            var solutionGUID = "{" + Guid.NewGuid().ToString().ToUpper() + "}";

            var solutionText = File.ReadAllText(vcSolutionPath);
            solutionText = string.Format(solutionText, projectName, projectGUID, solutionGUID);
            File.WriteAllText(Path.GetFullPath(Path.Combine(projectPath, $"{projectName}.sln")), solutionText);

            var projectText = File.ReadAllText(vcProjectPath);
            projectText = string.Format(projectText, projectName, projectGUID, ConstructListOfIncludePathsForMSVC(gadgetIncludes), gadgetLibraryPath);
            File.WriteAllText(Path.GetFullPath(Path.Combine(projectPath, @$"Code\{projectName}.vcxproj")), projectText);
        }

        private static string ConstructListOfIncludePathsForMSVC(List<string> includePaths)
        {
            Debug.Assert(includePaths != null && includePaths.Count > 0);

            string finalStr = "";
            foreach(var path in includePaths)
            {
                finalStr += path + ";";
            }

            return finalStr;
        }

        public NewProjectVM()
        {
            ProjectTemplates = new ReadOnlyObservableCollection<ProjectTemplate>(_projectTemplates);

            try
            {
                var templates = Directory.GetFiles(_templatePath, "template.xml", SearchOption.AllDirectories);
                Debug.Assert(templates != null && templates.Any());
                foreach (var file in templates)
                {
                    var filePath = Path.GetDirectoryName(file);
                    Debug.Assert(!string.IsNullOrWhiteSpace(filePath));

                    var template = Serializer.FromFile<ProjectTemplate>(file);
                    if (template == null)
                    {
                        Logger.Log(MessageType.Error, $"An error occured while deserializing {file}!");
                        throw new SerializationException($"An error occured while deserializing {file}!");
                    }

                    template.RootPath = filePath;
                    template.IconFilePath = Path.GetFullPath(Path.Combine(filePath, "icon.png"));
                    template.Icon = File.ReadAllBytes(template.IconFilePath);
                    template.ScreenshotFilePath = Path.GetFullPath(Path.Combine(filePath, "screenshot.png"));
                    template.Screenshot = File.ReadAllBytes(template.ScreenshotFilePath);
                    template.ProjectFilePath = Path.GetFullPath(Path.Combine(filePath, template.ProjectFile));

                    _projectTemplates.Add(template);
                }

            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, "Failed to read project templates!");
                throw;
            }

            ValidateProjectPath();
        }
    }
}