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

using Workbench.GadgetAPIStructs;

namespace Workbench.Editors.Components
{
    /// <summary>
    /// Interaction logic for CppComponentView.xaml
    /// </summary>
    public partial class CppComponentView : UserControl
    {
        private Action? _undoAction = null;
        private bool _propertyChanged = false;

        public CppComponentView()
        {
            InitializeComponent();
            Loaded += OnViewLoaded;
        }

        private void OnViewLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnViewLoaded;
            if (DataContext != null && DataContext is MultiSelectCppComponentVM msc)
            {
                msc.PropertyChanged += (s, e) => _propertyChanged = true;
            }
        }

        private Action? GetAction(Func<CppComponentVM, (CppComponentVM component, NamedVar)> selector, Action<(CppComponentVM component, NamedVar)> forEachAction)
        {
            if (DataContext is not MultiSelectCppComponentVM vm)
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

                (GameObjectView.Instance.DataContext as MultiSelectedObjectVM)?.GetMultiSelectComponent<MultiSelectCppComponentVM>()?.Refresh();
            });
        }

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
    }
}