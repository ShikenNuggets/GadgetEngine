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

namespace Workbench
{
    /// <summary>
    /// Interaction logic for OpenProjectControl.xaml
    /// </summary>
    public partial class OpenProjectControl : UserControl
    {
        public OpenProjectControl()
        {
            InitializeComponent();

            Loaded += (s, e) =>
            {
                var item = projectsListBox.ItemContainerGenerator.ContainerFromItem(projectsListBox.SelectedIndex) as ListBoxItem;
                if (item != null)
                {
                    item.Focus();
                }
            };
        }

        private void OnOpen_Button_Click(object sender, RoutedEventArgs e)
        {
            OpenSelectedProject();
        }

        private void OnListBoxItem_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            OpenSelectedProject();
        }

        private void OpenSelectedProject()
        {
            Debug.Assert(DataContext != null);
            Debug.Assert(DataContext is OpenProjectVM);

            var vm = DataContext as OpenProjectVM;
            Debug.Assert(vm != null);
            if (vm == null)
            {
                Logger.Log(MessageType.Error, "DataContext could not be converted to OpenProjectVM!");
                return;
            }

            if (projectsListBox == null || projectsListBox.SelectedItems.Count == 0)
            {
                Logger.Log(MessageType.Warning, "Open button clicked while no project templates are selected!");
                return;
            }

            var selectedItem = projectsListBox.SelectedItem;
            if (selectedItem == null || selectedItem is not ProjectData)
            {
                Logger.Log(MessageType.Warning, "Selected Item is not a valid ProjectData!");
                return;
            }
            var projectData = selectedItem as ProjectData;
            Debug.Assert(projectData != null);
            if (projectData == null)
            {
                Logger.Log(MessageType.Error, "Selected Item could not be converted to ProjectData!");
                return;
            }

            var project = OpenProjectVM.Open(projectData);

            bool dialogResult = false;
            var wnd = Window.GetWindow(this);
            if (project != null)
            {
                dialogResult = true;
                wnd.DataContext = project;
            }

            wnd.DialogResult = dialogResult;
            wnd.Close();
        }
    }
}
