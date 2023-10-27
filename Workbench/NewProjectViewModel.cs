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

namespace Workbench{
    [DataContract]
    public class ProjectTemplate
    {
        [DataMember] public required string ProjectType { get; set; }
        [DataMember] public required string ProjectFile {  get; set; }
        [DataMember] public required List<string> Folders { get; set; }

        public byte[]? Icon { get; set; }
        public byte[]? Screenshot { get; set; }
        public string? IconFilePath {  get; set; }
        public string? ScreenshotFilePath { get; set; }
        public string? ProjectFilePath {  get; set; }
    }

    class NewProjectViewModel : BaseViewModel
    {
        private readonly string _templatePath = @"ProjectTemplates\";

        private string _projectName = "New Project";
        private string _path = $@"{Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments)}\GadgetWorkbench\";

        public string ProjectName
        {
            get => _projectName;
            
            set
            {
                if (_projectName != value)
                {
                    _projectName = value;
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
                    OnPropertyChanged(nameof(Path));
                }
            }
        }

        private ObservableCollection<ProjectTemplate> _projectTemplates = new();
        public ReadOnlyObservableCollection<ProjectTemplate>? ProjectTemplates { get; }

        public NewProjectViewModel(){
            ProjectTemplates = new ReadOnlyObservableCollection<ProjectTemplate>(_projectTemplates);

            try
            {
                var templates = Directory.GetFiles(_templatePath, "template.xml", SearchOption.AllDirectories);
                Debug.Assert(templates != null && templates.Any());
                foreach (var file in templates)
                {
                    var filePath = System.IO.Path.GetDirectoryName(file);
                    Debug.Assert(!string.IsNullOrWhiteSpace(filePath));

                    var template = Serializer.FromFile<ProjectTemplate>(file);

                    template.IconFilePath = System.IO.Path.GetFullPath(System.IO.Path.Combine(filePath, "icon.png"));
                    template.Icon = File.ReadAllBytes(template.IconFilePath);
                    template.ScreenshotFilePath = System.IO.Path.GetFullPath(System.IO.Path.Combine(filePath, "screenshot.png"));
                    template.Screenshot = File.ReadAllBytes(template.ScreenshotFilePath);
                    template.ProjectFilePath = System.IO.Path.GetFullPath(System.IO.Path.Combine(filePath, template.ProjectFile));

                    _projectTemplates.Add(template);
                }

            }
            catch(Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }
    }
}