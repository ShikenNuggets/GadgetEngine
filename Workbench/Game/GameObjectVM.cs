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
        private ulong _guid = Utils.InvalidGUID;
        private bool _isActiveInEngine;
        private string _name;
        private bool _isEnabled = true;
        [DataMember (Name = nameof(Components))] private ObservableCollection<ComponentVM> _components = new ObservableCollection<ComponentVM>();

        [DataMember] public ulong GUID
        {
            get => _guid;
            set
            {
                if (_guid != value)
                {
                    _guid = value;
                    OnPropertyChanged(nameof(GUID));
                }
            }
        }
        [DataMember] public bool IsActiveInEngine
        {
            get => _isActiveInEngine;
            set
            {
                if (_isActiveInEngine != value)
                {
                    _isActiveInEngine = value;
                    if (_isActiveInEngine)
                    {
                        GUID = GadgetAPI.CreateGameObject(this);
                        Debug.Assert(GUID != Utils.InvalidGUID);
                    }
                    else if (GUID != Utils.InvalidGUID)
                    {
                        GadgetAPI.DestroyGameObject(this);
                        GUID = Utils.InvalidGUID;
                    }

                    OnPropertyChanged(nameof(IsActiveInEngine));
                }
            }
        }
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

        public ReadOnlyObservableCollection<ComponentVM>? Components { get; private set; } = null;

        public ComponentVM? GetComponent(Type type) => Components?.FirstOrDefault(c => c.GetType() == type);
        public T? GetComponent<T>() where T : ComponentVM => GetComponent(typeof(T)) as T;

        public GameObjectVM(SceneVM scene)
        {
            _name = "GameObject";

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

                Debug.Assert(e != null);
                Debug.Assert(!string.IsNullOrWhiteSpace(e.PropertyName));

                UpdateGameObjects(e.PropertyName);
            };
        }

        public T? GetMultiSelectComponent<T>() where T : IMultiSelectComponent
        {
            Debug.Assert(Components != null);
            return (T?)Components.FirstOrDefault(x => x.GetType() == typeof(T));
        }

        protected virtual bool UpdateGameObjects(string propertyName)
        {
            switch (propertyName)
            {
                case nameof(IsEnabled):
                    if (IsEnabled != null)
                    {
                        SelectedObjects.ForEach(x => x.IsEnabled = IsEnabled.Value);
                    }
                    return true;

                case nameof(Name):
                    if (Name != null)
                    {
                        SelectedObjects.ForEach(x => x.Name = Name);
                    }
                    return true;
            }

            return false;
        }

        public static float? GetMixedValues<T>(List<T> objects, Func<T, float> getProperty)
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

        public static bool? GetMixedValues<T>(List<T> objects, Func<T, bool> getProperty)
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

        public static string? GetMixedValues<T>(List<T> objects, Func<T, string> getProperty)
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
            MakeCommonComponentList();
            
            _canUpdate = true;
        }

        private void MakeCommonComponentList()
        {
            _components.Clear();
            var firstObj = SelectedObjects.FirstOrDefault();
            if (firstObj == null || firstObj.Components == null)
            {
                return;
            }

            foreach(var component in firstObj.Components)
            {
                var type = component.GetType();
                if (!SelectedObjects.Skip(1).Any(obj => obj.GetComponent(type) == null))
                {
                    Debug.Assert(Components.FirstOrDefault(x => x.GetType() == type) == null);
                    _components.Add(component.GetMultiSelectComponent(this));
                }
            }
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