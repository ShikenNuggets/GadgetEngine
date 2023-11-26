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
    enum BuildConfiguration
    {
        Editor,
        Win64_Debug,
        Win64_Release
    }

    [DataContract(Name = "Game")]
    public class ProjectVM : BaseViewModel
    {
        public const string ProjectExtension = ".wbn";
        public static string TempDir { get; } = $@".wbn\";

        private static readonly string[] _buildConfigurationNames = new string[] { "Debug", "EditorDLL", "Release" };
        private static string GetConfigurationName(BuildConfiguration config)
        {
            switch (config)
            {
                case BuildConfiguration.Editor:
                    return "EditorDLL";
                case BuildConfiguration.Win64_Debug:
                    return "Debug";
                case BuildConfiguration.Win64_Release:
                    return "Release";
                default:
                    Debug.Assert(false);
                    Logger.Log(MessageType.Error, $"Invalid build config {config}!");
                    return string.Empty;
            }
        }

        private BuildConfiguration _buildConfig;

        [DataMember] public string Name { get; private set; } = "NewProject";
        [DataMember] public string Path { get; private set; }
        [DataMember] public int BuildConfig
        {
            get => (int)_buildConfig;
            set
            {
                if ((int)_buildConfig != value)
                {
                    _buildConfig = (BuildConfiguration)value;
                    OnPropertyChanged(nameof(BuildConfig));
                }
            }
        }

        public string FullPath => GetFullPath(Path, Name);
        public string SolutionPath => GetFullPath(Path, Name, VisualStudioHelper.SolutionExtension);

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

        public static bool HasUnsavedChanges => !UndoRedo.IsAtCheckpoint;

        public static void TryShowUnsavedChanges()
        {
            if (Application.Current != null && Application.Current.MainWindow != null && Application.Current.MainWindow is MainWindow mw)
            {
                mw.UpdateTitleToShowUnsavedChanges();
            }
        }

        public ICommand? UndoCommand {  get; private set; }
        public ICommand? RedoCommand {  get; private set; }
        public ICommand? SaveCommand {  get; private set; }
        public ICommand? AddSceneCommand {  get; private set; }
        public ICommand? RemoveSceneCommand {  get; private set; }
        public ICommand? BuildCommand {  get; private set; }

        private void SetupCommands()
        {
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

            UndoCommand = new RelayCommand<object>(x => UndoRedo.Undo(), x => UndoRedo.UndoList.Any());
            RedoCommand = new RelayCommand<object>(x => UndoRedo.Redo(), x => UndoRedo.RedoList.Any());
            SaveCommand = new RelayCommand<object>(x => Save(this));
            BuildCommand = new RelayCommand<bool>(async x => await BuildGameCodeDLL(x), x => !VisualStudioHelper.IsBusy() && VisualStudioHelper.BuildDone);

            OnPropertyChanged(nameof(AddSceneCommand));
            OnPropertyChanged(nameof(RemoveSceneCommand));
            OnPropertyChanged(nameof(UndoCommand));
            OnPropertyChanged(nameof(RedoCommand));
            OnPropertyChanged(nameof(SaveCommand));
            OnPropertyChanged(nameof(BuildCommand));
        }

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
            UndoRedo.SetCheckpoint();
        }

        private async Task BuildGameCodeDLL(bool showWindow = true)
        {
            try
            {
                UnloadGameCodeDLL();

                await Task.Run(() => VisualStudioHelper.BuildSolution(this, GetConfigurationName(BuildConfiguration.Editor), showWindow));

                if (VisualStudioHelper.LastBuildSucceeded)
                {
                    await Task.Run(LoadGameCodeDLL);
                }
                else
                {
                    Logger.Log(MessageType.Warning, "Build failed, DLL will not be loaded.");
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, "An error occurred while trying to build the game code DLL!");
                throw;
            }
        }

        private async void LoadGameCodeDLL()
        {
            var dll = $@"{Path}Build\EditorDLL\{Name}.dll";
            if (!File.Exists(dll))
            {
                return;
            }

            int result = await Task.Run(() => GadgetAPI.LoadGameCodeDLL(dll));

            if (result != 0) 
            {
                Logger.Log(MessageType.Info, $"{Name}.dll loaded successfully.");
            }
            else
            {
                Logger.Log(MessageType.Warning, $"{Name}.dll could not be loaded!");
            }
        }

        private void UnloadGameCodeDLL()
        {
            if (GadgetAPI.UnloadGameCodeDLL() != 0)
            {
                Logger.Log(MessageType.Info, $"{Name}.dll unloaded.");
            }
            else
            {
                Logger.Log(MessageType.Warning, $"{Name}.dll could not be unloaded!");
            }
        }

        private ProjectVM(string name, string path)
        {
            Name = name;
            Path = path;

            Scenes = new ReadOnlyObservableCollection<SceneVM>(_scenes);

            OnDeserialized(new StreamingContext());
        }

        [OnDeserialized]
        private async void OnDeserialized(StreamingContext context)
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

            await Task.Run(LoadGameCodeDLL);

            SetupCommands();
        }

        public void Unload()
        {
            VisualStudioHelper.CloseVisualStudio();
            UndoRedo.Reset();
        }

        public static string GetFullPath(string path, string name, string extension = ProjectExtension)
        {
            return $@"{path}\{name}{extension}";
        }

        public string GetCppNamespaceName()
        {
            Debug.Assert(!string.IsNullOrWhiteSpace(Name));

            var projectName = Name.Replace(' ', '_');
            return projectName;
        }

        public string GetCppPreprocessorPrefix()
        {
            return GetCppNamespaceName().ToUpper() + "_";
        }
    }
}