using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
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
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public static string GadgetEnginePath { get; private set; } = Directory.GetCurrentDirectory() + @"\_Gadget\";

        public MainWindow()
        {
            InitializeComponent();
            Loaded += OnMainWindowLoaded;
            Closing += OnMainWindowClosing;
        }

        private void OnMainWindowLoaded(object sender, RoutedEventArgs e)
        {
            this.Loaded -= OnMainWindowLoaded;
            OpenProjectBrowser();
        }

        private void OnMainWindowClosing(object? sender, CancelEventArgs e)
        {
            Closing -= OnMainWindowClosing;
            ProjectVM.Current?.Unload();
        }

        private void OpenProjectBrowser()
        {
            var projectBrowser = new ProjectBrowserWindow();
            if (projectBrowser.ShowDialog() == false || projectBrowser.DataContext == null)
            {
                Application.Current.Shutdown();
            }
            else
            {
                if(ProjectVM.Current != null) 
                {
                    ProjectVM.Current.Unload();
                }

                DataContext = projectBrowser.DataContext;
            }
        }
    }
}
