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
            Debug.Assert(sender is ListBox);
            var gameObject = (sender as ListBox).SelectedItems[0];
            GameObjectView.Instance.DataContext = gameObject;
        }
    }
}
