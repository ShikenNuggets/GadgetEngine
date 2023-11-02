using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Workbench
{
    [DataContract]
    [KnownType(typeof(TransformComponentVM))]
    public class GameObjectVM : BaseViewModel
    {
        private string _name;
        private bool _isEnabled = true;
        [DataMember (Name = nameof(Components))] private ObservableCollection<ComponentVM> _components = new ObservableCollection<ComponentVM>();

        [DataMember] public string Name
        {
            get => _name;
            set
            {
                if (_name != value)
                {
                    _name = value;
                    OnPropertyChanged(nameof(Name));
                }
            }
        }
        [DataMember] public bool IsEnabled
        {
            get => _isEnabled;
            set
            {
                if (_isEnabled != value)
                {
                    _isEnabled = value;
                    OnPropertyChanged(nameof(IsEnabled));
                }
            }
        }
        [DataMember] public SceneVM ParentScene { get; private set; }

        public ReadOnlyObservableCollection<ComponentVM> Components { get; private set; }

        public GameObjectVM(SceneVM scene)
        {
            Debug.Assert(scene != null);
            ParentScene = scene;
            _components.Add(new TransformComponentVM(this));

            OnDeserialized(new StreamingContext());
        }

        [OnDeserialized]
        void OnDeserialized(StreamingContext context)
        {
            if (_components == null)
            {
                //TODO - Indicate some kind of "corrupted project" error to the user
                _components = new ObservableCollection<ComponentVM>
                {
                    new TransformComponentVM(this)
                };
            }

            Components = new ReadOnlyObservableCollection<ComponentVM>(_components);
            OnPropertyChanged(nameof(Components));
        }
    }

    public abstract class MultiSelectedObjectVM : BaseViewModel
    {
        private bool _canUpdate = true;

        private bool? _isEnabled;
        private string? _name;
        private readonly ObservableCollection<IMultiSelectComponent> _components = new ObservableCollection<IMultiSelectComponent>();

        public bool? IsEnabled
        {
            get => _isEnabled;
            set
            {
                if (_isEnabled != value)
                {
                    _isEnabled = value;
                    OnPropertyChanged(nameof(IsEnabled));
                }
            }
        }

        public string? Name
        {
            get => _name;
            set
            {
                if (_name != value)
                {
                    _name = value;
                    OnPropertyChanged(nameof(Name));
                }
            }
        }

        public ReadOnlyObservableCollection<IMultiSelectComponent> Components { get; private set; }

        public List<GameObjectVM> SelectedObjects { get; private set; }

        public ICommand RenameCommand { get; private set; }
        public ICommand EnableCommand { get; private set; }

        public MultiSelectedObjectVM(List<GameObjectVM> objects)
        {
            Debug.Assert(objects != null && objects.Any() == true);
            Components = new ReadOnlyObservableCollection<IMultiSelectComponent>(_components);
            SelectedObjects = new List<GameObjectVM>(objects);

            PropertyChanged += (s, e) =>
            {
                if (!_canUpdate)
                {
                    return;
                }

                UpdateGameObjects(e.PropertyName);
            };

            RenameCommand = new RelayCommand<string>(x =>
            {
                Debug.Assert(!string.IsNullOrEmpty(x));
                var oldName = _name;
                Name = x;

                ProjectVM.UndoRedo.Add(new UndoRedoAction($"Renamed GameObject '{oldName}' to '{x}'", nameof(Name), this, oldName, x));
            }, x => x != _name);

            EnableCommand = new RelayCommand<bool>(x =>
            {
                var oldValue = _isEnabled;
                IsEnabled = x;

                ProjectVM.UndoRedo.Add(new UndoRedoAction(x ? $"Enabled '{Name}'" : $"Disabled '{Name}'", nameof(IsEnabled), this, oldValue, x));
            });
        }

        protected virtual bool UpdateGameObjects(string propertyName)
        {
            switch (propertyName)
            {
                case nameof(IsEnabled): SelectedObjects.ForEach(x => x.IsEnabled = IsEnabled.Value); return true;
                case nameof(Name): SelectedObjects.ForEach(x => x.Name = Name); return true;
            }

            return false;
        }

        public static float? GetMixedValues(List<GameObjectVM> objects, Func<GameObjectVM, float> getProperty)
        {
            var value = getProperty(objects.First());
            foreach(var obj in objects.Skip(1))
            {
                if (!Utils.Near(value, getProperty(obj)))
                {
                    return null;
                }
            }

            return value;
        }

        public static bool? GetMixedValues(List<GameObjectVM> objects, Func<GameObjectVM, bool> getProperty)
        {
            var value = getProperty(objects.First());
            foreach (var obj in objects.Skip(1))
            {
                if (value != getProperty(obj))
                {
                    return null;
                }
            }

            return value;
        }

        public static string? GetMixedValues(List<GameObjectVM> objects, Func<GameObjectVM, string> getProperty)
        {
            var value = getProperty(objects.First());
            foreach (var obj in objects.Skip(1))
            {
                if (value != getProperty(obj))
                {
                    return null;
                }
            }

            return value;
        }

        protected virtual bool UpdateMultiSelectGameEntity()
        {
            IsEnabled = GetMixedValues(SelectedObjects, new Func<GameObjectVM, bool>(x => x.IsEnabled));
            Name = GetMixedValues(SelectedObjects, new Func<GameObjectVM, string>(x => x.Name));

            return true;
        }

        public void Refresh()
        {
            _canUpdate = false;

            UpdateMultiSelectGameEntity();
            
            _canUpdate = true;
        }
    }

    public class MultiSelectedGameObjectVM : MultiSelectedObjectVM
    {
        public MultiSelectedGameObjectVM(List<GameObjectVM> gameObjectVMs) : base(gameObjectVMs)
        {
            Refresh();
        }
    }
}