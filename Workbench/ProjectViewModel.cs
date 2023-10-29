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
    public class ProjectViewModel : BaseViewModel
    {
        public static string Extension { get; } = ".wbn";
        public static string TempDir { get; } = $@".wbn\";

        [DataMember] public string Name { get; private set; } = "NewProject";
        [DataMember] public string Path { get; private set; }

        public string FullPath => GetFullPath(Path, Name);

        [DataMember(Name = "Scenes")] private ObservableCollection<SceneViewModel> _scenes = new ObservableCollection<SceneViewModel>();
        [DataMember(Name = "ActiveScene")] private SceneViewModel _activeScene;

        public ReadOnlyObservableCollection<SceneViewModel> Scenes { get; private set; }

        public SceneViewModel ActiveScene
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

        public static ProjectViewModel Current => Application.Current.MainWindow.DataContext as ProjectViewModel;

        public static ProjectViewModel Load(string file)
        {
            Debug.Assert(File.Exists(file));
            return Serializer.FromFile<ProjectViewModel>(file);
        }

        public static void Save(ProjectViewModel project)
        {
            Serializer.ToFile(project, project.FullPath);
        }

        private ProjectViewModel(string name, string path)
        {
            Name = name;
            Path = path;

            OnDeserialized(new StreamingContext());
        }

        [OnDeserialized]
        private void OnDeserialized(StreamingContext context)
        {
            if (_scenes == null)
            {
                _scenes = new ObservableCollection<SceneViewModel>
                {
                    new SceneViewModel(this, "DefaultScene")
                };
            }

            Scenes = new ReadOnlyObservableCollection<SceneViewModel>(_scenes);
            OnPropertyChanged(nameof(Scenes));

            ActiveScene = Scenes.FirstOrDefault(x => x.IsActive);
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
