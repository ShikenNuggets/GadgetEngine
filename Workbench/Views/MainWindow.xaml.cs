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
        private static string _baseWindowTitle => "Gadget Workbench";
        private static string WindowTitle => ProjectVM.HasUnsavedChanges ? _baseWindowTitle + "*" : _baseWindowTitle;

        public static string GadgetEnginePath { get; private set; } = Directory.GetCurrentDirectory() + @"\_Gadget\";

        public MainWindow()
        {
            InitializeComponent();
            Loaded += OnMainWindowLoaded;
            Closing += OnMainWindowClosing;

            GadgetAPI.InitForWorkbench();
        }

        public void UpdateTitleToShowUnsavedChanges()
        {
            Title = WindowTitle; //This isn't really the proper way to do this, but it's easier than dealing with properties and stuff
        }

        private void OnMainWindowLoaded(object sender, RoutedEventArgs e)
        {
            this.Loaded -= OnMainWindowLoaded;
            OpenProjectBrowser();
        }

        private void OnMainWindowClosing(object? sender, CancelEventArgs e)
        {
            if (ProjectVM.HasUnsavedChanges)
            {
                bool? result = new UnsavedChangesView().ShowDialog();
                if (result == false)
                {
                    e.Cancel = true;
                    return; //User changed their mind, don't exit
                }
            }

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
