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
    /// Interaction logic for ProjectLayoutView.xaml
    /// </summary>
    public partial class ProjectLayoutView : UserControl
    {
        public ProjectLayoutView()
        {
            InitializeComponent();
        }

        private void OnAddGameObject_Button_Click(object sender, RoutedEventArgs e)
        {
            Debug.Assert(DataContext != null);
            Debug.Assert(DataContext is SceneVM);
            Debug.Assert(sender != null);
            Debug.Assert(sender is Button);

            if (sender is not Button btn)
            {
                Logger.Log(MessageType.Warning, "Invalid sender for Button Click callback!");
                return;
            }

            if (btn.DataContext is not SceneVM vm || vm.GameObjects == null)
            {
                return;
            }

            vm.AddGameObjectCommand.Execute(new GameObjectVM(vm) { Name = "GameObject" + vm.GameObjects.Count});
        }

        private void OnGameObject_ListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (sender == null)
            {
                return;
            }

            Debug.Assert(sender is ListBox);

            var listBox = sender as ListBox;
            Debug.Assert(listBox != null);
            if(listBox == null)
            {
                Logger.Log(MessageType.Warning, "Invalid sender for selection changed callback!");
                return;
            }

            var newSelection = listBox.SelectedItems.Cast<GameObjectVM>().ToList();
            var previousSelection = newSelection.Except(e.AddedItems.Cast<GameObjectVM>()).Concat(e.RemovedItems.Cast<GameObjectVM>()).ToList();

            ProjectVM.UndoRedo.Add(new UndoRedoAction(
                "GameObject Selection Changed",
                () => { //Undo
                    listBox.UnselectAll();
                    previousSelection.ForEach(x => (listBox.ItemContainerGenerator.ContainerFromItem(x) as ListBoxItem).IsSelected = true);
                },
                () => { //Redo
                    listBox.UnselectAll();
                    newSelection.ForEach(x => (listBox.ItemContainerGenerator.ContainerFromItem(x) as ListBoxItem).IsSelected = true);
                }
            ));

            MultiSelectedGameObjectVM? msGo = null;
            if (newSelection.Any())
            {
                msGo = new MultiSelectedGameObjectVM(newSelection);
            }

            Debug.Assert(GameObjectView.Instance != null);
            if (GameObjectView.Instance != null)
            {
                GameObjectView.Instance.DataContext = msGo;
            }
        }
    }
}
