using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
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

namespace Workbench.Editors.Components
{
    /// <summary>
    /// Interaction logic for TransformComponentView.xaml
    /// </summary>
    public partial class TransformComponentView : UserControl
    {
        private Action? _undoAction = null;
        private bool _propertyChanged = false;

        public TransformComponentView()
        {
            InitializeComponent();
            Loaded += OnViewLoaded;
        }

        private void OnViewLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnViewLoaded;
            if (DataContext != null && DataContext is MultiSelectTransformVM mst)
            {
                mst.PropertyChanged += (s, e) => _propertyChanged = true;
            }
        }

        private Action? GetAction(Func<TransformComponentVM, (TransformComponentVM transform, Vector3)> selector, Action<(TransformComponentVM transform, Vector3)> forEachAction)
        {
            if (!(DataContext is MultiSelectTransformVM vm))
            {
                _undoAction = null;
                _propertyChanged = false;
                return null;
            }

            var selection = vm.SelectedComponents.Select(selector).ToList();
            return new Action(() =>
            {
                selection.ForEach(x => forEachAction(x));

                Debug.Assert(GameObjectView.Instance != null);
                Debug.Assert(GameObjectView.Instance.DataContext != null);
                Debug.Assert(GameObjectView.Instance.DataContext is MultiSelectedObjectVM mso);

                (GameObjectView.Instance.DataContext as MultiSelectedObjectVM)?.GetMultiSelectComponent<MultiSelectTransformVM>()?.Refresh();
            });
        }

        private Action? GetPositionAction() => GetAction((x) => (x, x.Position), (x) => x.transform.Position = x.Item2);
        private Action? GetRotationAction() => GetAction((x) => (x, x.Rotation), (x) => x.transform.Rotation = x.Item2);
        private Action? GetScaleAction() => GetAction((x) => (x, x.Scale), (x) => x.transform.Scale = x.Item2);

        private void RecordAction(string name, Action? redoAction)
        {
            if (_propertyChanged)
            {
                Debug.Assert(_undoAction != null);
                Debug.Assert(redoAction != null);

                _propertyChanged = false;

                ProjectVM.UndoRedo.Add(new UndoRedoAction(name, _undoAction, redoAction));
            }
        }

        private void OnPosition_VectorBox_Preview_Mouse_LeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            _propertyChanged = false;
            _undoAction = GetPositionAction();
        }

        private void OnRotation_VectorBox_Preview_Mouse_LeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            _propertyChanged = false;
            _undoAction = GetRotationAction();
        }

        private void OnScale_VectorBox_Preview_Mouse_LeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            _propertyChanged = false;
            _undoAction = GetScaleAction();
        }

        private void OnPosition_VectorBox_Preview_Mouse_LeftButtonUp(object sender, MouseButtonEventArgs? e)
        {
            if (DataContext is not MultiSelectTransformVM vm)
            {
                return;
            }

            var selection = vm.SelectedComponents.Select(t => (t, t.Position)).ToList();
            RecordAction(selection.Count == 1 ? "Moved 1 GameObject" : $"Moved {selection.Count} GameObjects", GetPositionAction());
        }

        private void OnRotation_VectorBox_Preview_Mouse_LeftButtonUp(object sender, MouseButtonEventArgs? e)
        {
            if (DataContext is not MultiSelectTransformVM vm)
            {
                return;
            }

            var selection = vm.SelectedComponents.Select(t => (t, t.Position)).ToList();
            RecordAction(selection.Count == 1 ? "Rotated 1 GameObject" : $"Rotated {selection.Count} GameObjects", GetRotationAction());
        }

        private void OnScale_VectorBox_Preview_Mouse_LeftButtonUp(object sender, MouseButtonEventArgs? e)
        {
            if (DataContext is not MultiSelectTransformVM vm)
            {
                return;
            }

            var selection = vm.SelectedComponents.Select(t => (t, t.Position)).ToList();
            RecordAction(selection.Count == 1 ? "Scaled 1 GameObject" : $"Scaled {selection.Count} GameObjects", GetScaleAction());
        }

        private void OnPosition_VectorBox_Preview_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            if(_propertyChanged && _undoAction != null)
            {
                OnPosition_VectorBox_Preview_Mouse_LeftButtonUp(sender, null);
            }
        }
        private void OnRotation_VectorBox_Preview_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            if (_propertyChanged && _undoAction != null)
            {
                OnRotation_VectorBox_Preview_Mouse_LeftButtonUp(sender, null);
            }
        }
        private void OnScale_VectorBox_Preview_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            if (_propertyChanged && _undoAction != null)
            {
                OnScale_VectorBox_Preview_Mouse_LeftButtonUp(sender, null);
            }
        }
    }
}
