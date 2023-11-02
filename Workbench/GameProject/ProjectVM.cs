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
using System.Windows.Input;

namespace Workbench
{
    [DataContract(Name = "Game")]
    public class ProjectVM : BaseViewModel
    {
        public static string Extension { get; } = ".wbn";
        public static string TempDir { get; } = $@".wbn\";

        [DataMember] public string Name { get; private set; } = "NewProject";
        [DataMember] public string Path { get; private set; }

        public string FullPath => GetFullPath(Path + $@"{Name}", Name);

        [DataMember(Name = "Scenes")] private ObservableCollection<SceneVM> _scenes = new ObservableCollection<SceneVM>();
        [DataMember(Name = "ActiveScene")] private SceneVM? _activeScene;

        public ReadOnlyObservableCollection<SceneVM> Scenes { get; private set; }

        public SceneVM? ActiveScene
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

        public static ProjectVM? Current => Application.Current.MainWindow.DataContext as ProjectVM;

        public static UndoRedo UndoRedo { get; private set; } = new UndoRedo();

        public ICommand? UndoCommand {  get; private set; }
        public ICommand? RedoCommand {  get; private set; }
        public ICommand? SaveCommand {  get; private set; }
        public ICommand? AddSceneCommand {  get; private set; }
        public ICommand? RemoveSceneCommand {  get; private set; }

        private SceneVM AddSceneInternal(string sceneName)
        {
            Debug.Assert(!string.IsNullOrWhiteSpace(sceneName));
            SceneVM scene = new(this, sceneName);
            _scenes.Add(scene);
            return scene;
        }

        private void RemoveSceneInternal(SceneVM scene)
        {
            Debug.Assert(scene != null && _scenes.Contains(scene));
            _scenes.Remove(scene);
        }

        public static ProjectVM Load(string file)
        {
            Debug.Assert(File.Exists(file));
            return Serializer.FromFile<ProjectVM>(file);
        }

        public static void Save(ProjectVM project)
        {
            Serializer.ToFile(project, project.FullPath);
            Logger.Log(MessageType.Info, $"Project saved to {project.FullPath}");
        }

        private ProjectVM(string name, string path)
        {
            Name = name;
            Path = path;

            Scenes = new ReadOnlyObservableCollection<SceneVM>(_scenes);

            OnDeserialized(new StreamingContext());
        }

        [OnDeserialized]
        private void OnDeserialized(StreamingContext context)
        {
            Logger.Log(MessageType.Verbose, "Verbose");
            Logger.Log(MessageType.Info, "Info");
            Logger.Log(MessageType.Warning, "Warning");
            Logger.Log(MessageType.Error, "Error");

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

            AddSceneCommand = new RelayCommand<object>(x =>
            {
                var newScene = AddSceneInternal($"Scene{_scenes.Count}");
                var sceneIndex = _scenes.IndexOf(newScene);

                UndoRedo.Add(new UndoRedoAction(
                    $"Added '{newScene.Name}'",
                    () => RemoveSceneInternal(newScene), //Undo
                    () => _scenes.Insert(sceneIndex, newScene) //Redo
                ));
            });

            RemoveSceneCommand = new RelayCommand<SceneVM>(x =>
            {
                Debug.Assert(x != null);
                if (x == null)
                {
                    throw new ArgumentNullException(nameof(x), "Invalid argument in RemoveSceneCommand!");
                }

                var sceneIndex = _scenes.IndexOf(x);
                RemoveSceneInternal(x);

                UndoRedo.Add(new UndoRedoAction(
                    $"Removed '{x.Name}'",
                    () => _scenes.Insert(sceneIndex, x),
                    () => RemoveSceneInternal(x)
                ));
            }, x => x != null && !x.IsActive);

            UndoCommand = new RelayCommand<object>(x => UndoRedo.Undo());
            RedoCommand = new RelayCommand<object>(x => UndoRedo.Redo());
            SaveCommand = new RelayCommand<object>(x => Save(this));
        }

        public void Unload()
        {
            UndoRedo.Reset();
        }

        public static string GetFullPath(string path, string name)
        {
            return $@"{path}\{name}{Extension}";
        }
    }
}