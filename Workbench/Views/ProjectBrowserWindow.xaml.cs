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
using System.Windows.Shapes;

namespace Workbench
{
    /// <summary>
    /// Interaction logic for ProjectBrowser.xaml
    /// </summary>
    public partial class ProjectBrowserWindow : Window
    {
        public ProjectBrowserWindow()
        {
            InitializeComponent();

            Loaded += OnProjectBrowserDialogLoaded;
        }

        private void OnProjectBrowserDialogLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnProjectBrowserDialogLoaded;
            if (!OpenProjectVM.Projects.Any())
            {
                OnToggleButton_Click(newProjectButton, new RoutedEventArgs());
            }
        }

        private void OnToggleButton_Click(object sender, RoutedEventArgs e)
        {
            if(sender == openProjectButton){
                newProjectButton.IsChecked = false;
                openProjectButton.IsChecked = true;

                openProjectArea.Visibility = Visibility.Visible;

            }else if(sender == newProjectButton){
                openProjectButton.IsChecked = false;
                newProjectButton.IsChecked = true;

                openProjectArea.Visibility = Visibility.Collapsed;
            }
        }
    }
}
