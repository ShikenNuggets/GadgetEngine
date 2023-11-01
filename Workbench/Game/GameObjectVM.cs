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

        public ICommand RenameCommand { get; private set; }
        public ICommand EnableCommand {  get; private set; }

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
    }
}