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
    /// Interaction logic for NewProjectControl.xaml
    /// </summary>
    public partial class NewProjectControl : UserControl
    {
        public NewProjectControl()
        {
            InitializeComponent();
        }

        private void OnCreate_Button_Click(object sender, RoutedEventArgs e)
        {
            Debug.Assert(DataContext != null);
            Debug.Assert(DataContext is NewProjectVM);

            if(DataContext == null)
            {
                Logger.Log(MessageType.Warning, "Create button clicked with no DataContext!");
                return;
            }

            var vm = DataContext as NewProjectVM;
            Debug.Assert(vm != null);
            if(vm == null)
            {
                Logger.Log(MessageType.Error, "DataContext could not be converted to NewProjectVM!");
                return;
            }

            if (templateListBox == null || templateListBox.SelectedItems.Count == 0)
            {
                Logger.Log(MessageType.Warning, "Create button clicked while no project templates are selected!");
                return;
            }

            var selectedItem = templateListBox.SelectedItems;
            if (selectedItem == null || selectedItem is not ProjectTemplate)
            {
                Logger.Log(MessageType.Warning, "Selected Item is not a valid ProjectTemplate!");
                return;
            }
            var projectTemplate = selectedItem as ProjectTemplate;
            Debug.Assert(projectTemplate != null);
            if (projectTemplate == null)
            {
                Logger.Log(MessageType.Error, "Selected Item could not be converted to ProjectTemplate!");
                return;
            }

            var projectPath = vm.CreateProject(projectTemplate);

            bool dialogResult = false;
            var wnd = Window.GetWindow(this);
            if(!string.IsNullOrWhiteSpace(projectPath))
            {
                dialogResult = true;
                var project = OpenProjectVM.Open(new ProjectData(){ 
                    ProjectName = vm.ProjectName, 
                    ProjectPath = projectPath 
                });
                wnd.DataContext = project;
            }

            wnd.DialogResult = dialogResult;
            wnd.Close();
        }
    }
}
