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
            var btn = sender as Button;
            var vm = btn.DataContext as SceneVM;
            vm.AddGameObjectCommand.Execute(new GameObjectVM(vm) { Name = "GameObject" + vm.GameObjects.Count});
        }

        private void OnGameObject_ListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            GameObjectView.Instance.DataContext = null;

            if (sender == null)
            {
                return;
            }
            Debug.Assert(sender is ListBox);

            var listBox = sender as ListBox;
            Debug.Assert(listBox != null);
            if (e.AddedItems.Count > 0)
            {
                GameObjectView.Instance.DataContext = listBox.SelectedItems[0]; //TODO - This sucks
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
        }
    }
}
