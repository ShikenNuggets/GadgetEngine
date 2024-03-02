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

using Workbench.Controls;
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

                var grid = Utils.GetChildOfType<Grid>(componentView);
                if (grid == null)
                {
                    return;
                }
                int currentRow = 1;

                foreach (var v in msc.Properties)
                {
                    RowDefinition rd = new RowDefinition();
                    rd.Height = new GridLength(30, GridUnitType.Auto);
                    grid.RowDefinitions.Add(rd);

                    TextBlock tb = new TextBlock();
                    grid.Children.Add(new TextBlock());
                    tb.Text = GadgetAPI.GetStringFromID(v.name);
                    tb.SetValue(Grid.RowProperty, currentRow);
                    tb.SetValue(Grid.ColumnProperty, 0);

                    switch (v.value.type)
                    {
                        case (int)Var.VarType.Null:
                            ScalarBox sb = new();
                            sb.Value = null;
                            sb.SetValue(Grid.RowProperty, currentRow);
                            sb.SetValue(Grid.ColumnProperty, 1);
                            grid.Children.Add(sb);
                            break;
                        case (int)Var.VarType.String:
                            TextBox tBox = new();
                            tBox.Text = GadgetAPI.GetStringFromID(v.value.strVal);
                            tBox.SetValue(Grid.RowProperty, currentRow);
                            tBox.SetValue(Grid.ColumnProperty, 1);
                            grid.Children.Add(tBox);
                            break;
                        case (int)Var.VarType.Bool:
                            CheckBox cb = new();
                            cb.IsChecked = v.value.boolVal;
                            cb.SetValue(Grid.RowProperty, currentRow);
                            cb.SetValue(Grid.ColumnProperty, 1);
                            grid.Children.Add(cb);
                            break;
                        case (int)Var.VarType.Number:
                            ScalarBox sb2 = new();
                            sb2.Value = v.value.numVal.ToString("0.#####");
                            sb2.SetValue(Grid.RowProperty, currentRow);
                            sb2.SetValue(Grid.ColumnProperty, 1);
                            grid.Children.Add(sb2);
                            break;
                    }

                    currentRow++;
                }
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