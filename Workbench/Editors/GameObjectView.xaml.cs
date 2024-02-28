using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Workbench.Editors
{
    /// <summary>
    /// Interaction logic for GameObjectView.xaml
    /// </summary>
    public partial class GameObjectView : UserControl
    {
        private Action? _undoAction = null;
        private string? _propertyName = null;

        public static GameObjectView? Instance { get; private set; }

        public GameObjectView()
        {
            Debug.Assert(Instance == null);

            InitializeComponent();
            DataContext = null;
            Instance = this;
            DataContextChanged += (_, __) =>
            {
                if (DataContext == null)
                {
                    return;
                }

                var mso = DataContext as MultiSelectedObjectVM;
                Debug.Assert(mso != null);
                if (mso != null)
                {
                   mso.PropertyChanged += (s, e) => _propertyName = e.PropertyName;
                }
            };
        }

        private Action GetIsEnabledAction()
        {
            var vm = DataContext as MultiSelectedObjectVM;

            Debug.Assert(vm != null);

            var selection = vm.SelectedObjects.Select(obj => (obj, obj.IsEnabled)).ToList();
            return new Action(() =>
            {
                selection.ForEach(item => item.obj.IsEnabled = item.IsEnabled);
                vm.Refresh();
            });
        }

        private Action GetRenameAction()
        {
            var vm = DataContext as MultiSelectedObjectVM;

            Debug.Assert(vm != null);

            var selection = vm.SelectedObjects.Select(obj => (obj, obj.Name)).ToList();
            return new Action(() =>
            {
                selection.ForEach(item => item.obj.Name = item.Name);
                vm.Refresh();
            });
        }

        private void OnName_TextBox_GotKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            _propertyName = string.Empty;
            if (DataContext == null || DataContext is not MultiSelectedObjectVM)
            {
                return;
            }

            _undoAction = GetRenameAction();
            _propertyName = nameof(Name);
        }

        private void OnName_TextBox_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            Debug.Assert(sender != null);
            Debug.Assert(sender is TextBox);
            if (DataContext == null || DataContext is not MultiSelectedObjectVM)
            {
                _undoAction = null;
                return;
            }

            if (_propertyName == nameof(MultiSelectedObjectVM.Name) && _undoAction != null)
            {
                var vm = DataContext as MultiSelectedObjectVM;
                Debug.Assert(vm != null);
                if (vm == null)
                {
                    Logger.Log(MessageType.Error, "DataContext could not be converted to MultiSelectedObjectVM!");
                    return;
                }

                var numSelectedObjs = vm.SelectedObjects.Count;
                    
                var redoAction = GetRenameAction();
                ProjectVM.UndoRedo.Add(new UndoRedoAction(numSelectedObjs == 1 ? $"Renamed 1 GameObject" : $"Renamed {numSelectedObjs} GameObjects", _undoAction, redoAction));
                _propertyName = null;
            }
            _undoAction = null;
        }

        private void OnIsEnabled_Checkbox_Click(object sender, RoutedEventArgs e)
        {
            Debug.Assert(sender != null);
            Debug.Assert(sender is CheckBox);
            if (DataContext == null || DataContext is not MultiSelectedObjectVM)
            {
                return;
            }

            var vm = DataContext as MultiSelectedObjectVM;
            Debug.Assert(vm != null);
            if (vm == null)
            {
                Logger.Log(MessageType.Error, "DataContext could not be converted to MultiSelectedObjectVM!");
                return;
            }

            var checkbox = sender as CheckBox;
            if (checkbox == null)
            {
                Logger.Log(MessageType.Error, "Sender was not a valid CheckBox!");
                return;
            }

            var numSelectedObjs = vm.SelectedObjects.Count;

            var undoAction = GetIsEnabledAction();
            vm.IsEnabled = checkbox.IsChecked == true;
            var redoAction = GetIsEnabledAction();

            string msg = $"Enabled {numSelectedObjs} GameObject";
            if(!vm.IsEnabled.Value)
            {
                msg = $"Disabled {numSelectedObjs} GameObject";
            }

            if (numSelectedObjs > 1)
            {
                msg += "s";
            }

            ProjectVM.UndoRedo.Add(new UndoRedoAction(msg, undoAction, redoAction));
        }

        private void OnAddComponent_Button_Click(object sender, RoutedEventArgs e)
        {
            Debug.Assert(sender != null);
            Debug.Assert(sender is ToggleButton);
            Debug.Assert(componentTypeListBox != null);
            if (componentTypeListBox == null)
            {
                Logger.Log(MessageType.Error, "componentTypeListBox is null!");
                return;
            }

            if (componentTypeListBox.Visibility == Visibility.Visible)
            {
                componentTypeListBox.Visibility = Visibility.Collapsed;
            }
            else
            {
                componentTypeListBox.Visibility = Visibility.Visible;
            }
        }

        private void OnComponentType_ListBox_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            Debug.Assert(sender != null);
            Debug.Assert(sender is ListBox);
            if (DataContext == null || DataContext is not MultiSelectedObjectVM)
            {
                return;
            }

            var vm = DataContext as MultiSelectedGameObjectVM;
            Debug.Assert(vm != null);
            if (vm == null)
            {
                Logger.Log(MessageType.Error, "DataContext could not be converted to MultiSelectedObjectVM!");
                return;
            }

            var listBox = sender as ListBox;
            if (listBox == null || listBox.SelectedItem == null)
            {
                Logger.Log(MessageType.Error, "Sender was not a valid ListBox!");
                return;
            }

            Logger.Log(MessageType.Info, "Add component of type " + listBox.SelectedItem.ToString());
            GadgetAPI.AddNewComponentToGameObjects(vm, listBox.SelectedItem.ToString().Trim());

            listBox.Visibility = Visibility.Collapsed;
        }
    }
}
