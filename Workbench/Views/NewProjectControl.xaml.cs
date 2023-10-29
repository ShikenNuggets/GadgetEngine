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
            var vm = DataContext as NewProjectVM;
            var projectPath = vm.CreateProject(templateListBox.SelectedItem as ProjectTemplate);

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
