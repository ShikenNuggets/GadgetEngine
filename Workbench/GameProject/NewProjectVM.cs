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

                List<string> filesToIgnore = new()
                {
                    "template.xml",
                    template.ProjectFile,
                    Path.GetFileName(template.IconFilePath),
                    Path.GetFileName(template.ScreenshotFilePath)
                };

                Debug.Assert(!string.IsNullOrWhiteSpace(template.ProjectFilePath));

                //Copy all files from the template to the new directory
                Utils.CopyDirectory(new FileInfo(template.ProjectFilePath).Directory.FullName, finalPath, true, filesToIgnore);

                //Copy the project file, with the new name and new information
                var projectXml = File.ReadAllText(template.ProjectFilePath);
                projectXml = string.Format(projectXml, ProjectName, ProjectPath);
                var projectPath = ProjectVM.GetFullPath(finalPath, ProjectName);
                File.WriteAllText(projectPath, projectXml);

                //Copy the Icon and Screenshot to the temp folder
                File.Copy(template.IconFilePath, finalPath + ProjectVM.TempDir + Path.GetFileName(template.IconFilePath));
                File.Copy(template.ScreenshotFilePath, finalPath + ProjectVM.TempDir + Path.GetFileName(template.ScreenshotFilePath));

                return finalPath;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }

            return string.Empty;
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
            }

            ValidateProjectPath();
        }
    }
}