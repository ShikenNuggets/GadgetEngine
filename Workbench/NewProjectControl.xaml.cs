using System;
using System.Collections.Generic;
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
    /// Interaction logic for NewProjectController.xaml
    /// </summary>
    public partial class NewProjectController : UserControl
    {
        public NewProjectController()
        {
            InitializeComponent();
        }

        private void OnCreate_Button_Click(object sender, RoutedEventArgs e)
        {
            var vm = DataContext as NewProjectViewModel;
            var projectPath = vm.CreateProject(templateListBox.SelectedItem as ProjectTemplate);

            bool dialogResult = false;
            var wnd = Window.GetWindow(this);
            if(!string.IsNullOrWhiteSpace(projectPath))
            {
                dialogResult = true;
                var project = OpenProjectViewModel.Open(new ProjectData(){ 
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
