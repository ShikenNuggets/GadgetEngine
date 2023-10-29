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
    /// Interaction logic for OpenProjectControl.xaml
    /// </summary>
    public partial class OpenProjectControl : UserControl
    {
        public OpenProjectControl()
        {
            InitializeComponent();
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
            var vm = DataContext as OpenProjectVM;
            var project = OpenProjectVM.Open(projectsListBox.SelectedItem as ProjectData);

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
