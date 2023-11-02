using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
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
        private Action _undoAction;
        private string _propertyName;

        public static GameObjectView? Instance { get; private set; }

        public GameObjectView()
        {
            Debug.Assert(Instance == null);

            InitializeComponent();
            DataContext = null;
            Instance = this;
            DataContextChanged += (_, __) =>
            {
                Debug.Assert(DataContext != null && DataContext is MultiSelectedObjectVM);
                (DataContext as MultiSelectedObjectVM).PropertyChanged += (s, e) => _propertyName = e.PropertyName;
            };
        }

        private Action GetRenameAction()
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

        private Action GetIsEnabledAction()
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
            if (DataContext == null || DataContext is not MultiSelectedObjectVM)
            {
                return;
            }

            _undoAction = GetRenameAction();
            _propertyName = nameof(Name);
        }

        private void OnName_TextBox_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            if (DataContext == null || DataContext is not MultiSelectedObjectVM)
            {
                return;
            }

            if (_propertyName == nameof(MultiSelectedObjectVM.Name) && _undoAction != null)
            {
                var vm = DataContext as MultiSelectedObjectVM;
                var numSelectedObjs = vm.SelectedObjects.Count;

                var redoAction = GetRenameAction();
                ProjectVM.UndoRedo.Add(new UndoRedoAction(numSelectedObjs > 1 ? $"Renamed 1 GameObject" : $"Renamed {numSelectedObjs} GameObjects", _undoAction, redoAction));
                _propertyName = null;
            }
            _undoAction = null;
        }

        private void OnIsEnabled_Checkbox_Click(object sender, RoutedEventArgs e)
        {
            var vm = DataContext as MultiSelectedObjectVM;
            var numSelectedObjs = vm.SelectedObjects.Count;

            var undoAction = GetIsEnabledAction();
            vm.IsEnabled = (sender as CheckBox).IsChecked == true;
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
    }
}
