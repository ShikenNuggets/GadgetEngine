using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Workbench
{
    [DataContract(Name = "Game")]
    public class ProjectVM : BaseViewModel
    {
        public static string Extension { get; } = ".wbn";
        public static string TempDir { get; } = $@".wbn\";

        [DataMember] public string Name { get; private set; } = "NewProject";
        [DataMember] public string Path { get; private set; }

        public string FullPath => GetFullPath(Path, Name);

        [DataMember(Name = "Scenes")] private ObservableCollection<SceneVM> _scenes = new ObservableCollection<SceneVM>();
        [DataMember(Name = "ActiveScene")] private SceneVM _activeScene;

        public ReadOnlyObservableCollection<SceneVM> Scenes { get; private set; }

        public SceneVM ActiveScene
        {
            get => _activeScene;
            set
            {
                if (_activeScene != value)
                {
                    _activeScene = value;
                    OnPropertyChanged(nameof(ActiveScene));
                }
            }
        }

        public static ProjectVM Current => Application.Current.MainWindow.DataContext as ProjectVM;

        public static ProjectVM Load(string file)
        {
            Debug.Assert(File.Exists(file));
            return Serializer.FromFile<ProjectVM>(file);
        }

        public static void Save(ProjectVM project)
        {
            Serializer.ToFile(project, project.FullPath);
        }

        private ProjectVM(string name, string path)
        {
            Name = name;
            Path = path;

            OnDeserialized(new StreamingContext());
        }

        [OnDeserialized]
        private void OnDeserialized(StreamingContext context)
        {
            if (_scenes == null || _scenes.Count == 0)
            {
                _scenes = new ObservableCollection<SceneVM>
                {
                    new SceneVM(this, "DefaultScene")
                };
            }

            Scenes = new ReadOnlyObservableCollection<SceneVM>(_scenes);
            OnPropertyChanged(nameof(Scenes));

            ActiveScene = Scenes.FirstOrDefault(x => x.IsActive);
            if (ActiveScene == null)
            {
                Scenes.First().IsActive = true;
                ActiveScene = Scenes.First();
            }
        }

        public void Unload()
        {
            //TODO
        }

        public static string GetFullPath(string path, string name)
        {
            return $"{path}\\{name}{Extension}";
        }
    }
}
