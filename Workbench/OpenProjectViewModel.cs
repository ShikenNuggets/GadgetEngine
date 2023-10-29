using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace Workbench
{
    [DataContract]
    public class ProjectData
    {
        [DataMember] public string ProjectName { get; set; }
        [DataMember] public string ProjectPath { get; set; }
        [DataMember] public DateTime LastOpened { get; set; }

        public string FullPath { get => ProjectViewModel.GetFullPath(ProjectPath, ProjectName); }

        public byte[] Icon { get; set; }
        public byte[] Screenshot { get; set; }
    }

    [DataContract]
    public class ProjectDataList
    {
        [DataMember] public List<ProjectData> Projects { get; set; }
    }

    public class OpenProjectViewModel : BaseViewModel
    {
        private static readonly string _applicationDataPath = $@"{Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData)}\GadgetWorkbench\";
        private static readonly string _projectDataPath;
        private static readonly ObservableCollection<ProjectData> _projects = new ObservableCollection<ProjectData>();

        public static ReadOnlyObservableCollection<ProjectData> Projects {  get; }

        private static void ReadProjectData()
        {
            if (File.Exists(_projectDataPath))
            {
                var projects = Serializer.FromFile<ProjectDataList>(_projectDataPath).Projects.OrderByDescending(x => x.LastOpened);
                _projects.Clear();
                foreach(var project in projects)
                {
                    if (!File.Exists(project.FullPath))
                    {
                        continue;
                    }

                    project.Icon = File.ReadAllBytes($@"{project.ProjectPath}\{ProjectViewModel.TempDir}icon.png");
                    project.Screenshot = File.ReadAllBytes($@"{project.ProjectPath}\{ProjectViewModel.TempDir}screenshot.png");
                    _projects.Add(project);
                }
            }
        }

        private static void WriteProjectData()
        {
            var projects = _projects.OrderBy(x => x.LastOpened).ToList();
            Serializer.ToFile(new ProjectDataList() { Projects = projects }, _projectDataPath);
        }

        public static ProjectViewModel Open(ProjectData projectData)
        {
            ReadProjectData();
            var project = _projects.FirstOrDefault(x => x.FullPath == projectData.FullPath);
            if (project == null)
            {
                project = projectData;
                _projects.Add(project);
            }

            project.LastOpened = DateTime.Now;

            WriteProjectData();

            return ProjectViewModel.Load(project.FullPath);
        }

        static OpenProjectViewModel()
        {
            try
            {
                if (!Directory.Exists(_applicationDataPath))
                {
                    Directory.CreateDirectory(_applicationDataPath);
                }

                _projectDataPath = $@"{_applicationDataPath}ProjectData.xml";
                Projects = new ReadOnlyObservableCollection<ProjectData>(_projects);
                ReadProjectData();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }
    }
}