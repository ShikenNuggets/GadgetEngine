using System;
using System.Collections.Generic;
using System.Diagnostics;
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
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;

namespace Workbench
{
    /// <summary>
    /// Interaction logic for AddCppDialog.xaml
    /// </summary>
    public partial class AddCppDialog : Window
    {
        public AddCppDialog()
        {
            InitializeComponent();
            Owner = Application.Current.MainWindow;
            pathTextBox.Text = @"Code\";
        }

        private bool Validate()
        {
            Debug.Assert(ProjectVM.Current != null);
            Debug.Assert(ProjectVM.Current.Path != null);

            bool isValid = false;

            var name = classTextBox.Text.Trim();
            var path = pathTextBox.Text.Trim();

            var fullPath = Path.GetFullPath(Path.Combine(ProjectVM.Current.Path, path));
            var headerPath = Path.GetFullPath(Path.Combine(fullPath, $"{name}.h"));
            var cppPath = Path.GetFullPath(Path.Combine(fullPath, $"{name}.cpp"));

            string errorMessage = string.Empty;
            if (string.IsNullOrEmpty(name))
            {
                errorMessage = "You must enter a class name.";
            }
            else if (name.IndexOfAny(Path.GetInvalidFileNameChars()) != -1 || name.Any(x => char.IsWhiteSpace(x)))
            {
                errorMessage = "Invalid character(s) used in class name.";
            }
            else if (path.IndexOfAny(Path.GetInvalidPathChars()) != -1)
            {
                errorMessage = "Invalid character(s) used in file path.";
            }
            else if (!fullPath.Contains(Path.Combine(ProjectVM.Current.Path, @"Code\")))
            {
                errorMessage = "Class must be added to the 'Code' folder in the project (or a sub-folder).";
            }
            else if (File.Exists(headerPath) || File.Exists(cppPath))
            {
                errorMessage = $"Class {name} already exists.";
            }
            else
            {
                isValid = true;
            }

            if (!isValid)
            {
                messageTextBlock.Foreground = FindResource("Editor.RedBrush") as Brush;
            }
            else
            {
                messageTextBlock.Foreground = FindResource("Editor.FontBrush") as Brush;
            }

            messageTextBlock.Text = errorMessage;
            return isValid;
        }

        private void OnClass_TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            Debug.Assert(ProjectVM.Current != null);
            Debug.Assert(ProjectVM.Current.Name != null);

            if (!Validate())
            {
                return;
            }

            var name = classTextBox.Text.Trim();
            messageTextBlock.Text = $"{name}.h and {name}.cpp will be added to {ProjectVM.Current.Name}";
        }

        private void OnPath_TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            Validate();
        }

        private async void OnAdd_Button_Click(object sender, RoutedEventArgs e)
        {
            if (!Validate())
            {
                return;
            }

            Debug.Assert(ProjectVM.Current != null);
            Debug.Assert(!string.IsNullOrWhiteSpace(ProjectVM.Current.Name));
            Debug.Assert(!string.IsNullOrWhiteSpace(ProjectVM.Current.Path));
            Debug.Assert(!string.IsNullOrWhiteSpace(ProjectVM.Current.SolutionPath));

            IsEnabled = false; //Prevent the user from interacting with the window while we create the files asyncrhonously
            busyAnimationGrid.Visibility = Visibility.Visible;
            DoubleAnimation fadeIn = new DoubleAnimation(0, 1, new Duration(TimeSpan.FromMilliseconds(500)));
            busyAnimationGrid.BeginAnimation(OpacityProperty, fadeIn);

            try
            {
                var name = classTextBox.Text.Trim();
                var namespaceName = ProjectVM.Current.GetCppNamespaceName();
                var preprocessorPrefix = ProjectVM.Current.GetCppPreprocessorPrefix();
                var path = Path.GetFullPath(Path.Combine(ProjectVM.Current.Path, pathTextBox.Text.Trim()));
                var solution = ProjectVM.Current.SolutionPath;
                var projectName = ProjectVM.Current.Name;

                await Task.Run(() => CreateClass(name, namespaceName, preprocessorPrefix, path, solution, projectName));
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, $"Failed to create new CPP class {classTextBox.Text}");
            }
            finally
            {
                IsEnabled = true;

                DoubleAnimation fadeOut = new DoubleAnimation(1, 0, new Duration(TimeSpan.FromMilliseconds(200)));
                fadeOut.Completed += (s, e) =>
                {
                    busyAnimationGrid.Opacity = 0.0;
                    busyAnimationGrid.Visibility = Visibility.Hidden;
                    Close();
                };
                busyAnimationGrid.BeginAnimation(OpacityProperty, fadeOut);
            }
        }

        private static void CreateClass(string className, string namespaceName, string preprocessorPrefix, string path, string solution, string projectName)
        {
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }

            var headerPath = Path.GetFullPath(Path.Combine(path, $"{className}.h"));
            var cppPath = Path.GetFullPath(Path.Combine(path, $"{className}.cpp"));
            var preprocessor = preprocessorPrefix + className.Replace(' ', '_').Replace('.', '_').Trim().ToUpper() + "_H";

            using (var sw = File.CreateText(headerPath))
            {
                sw.Write(string.Format(CppTemplates.GameLogicComponentH, preprocessor, namespaceName, className));
            }

            using (var sw = File.CreateText(cppPath))
            {
                sw.Write(string.Format(CppTemplates.GameLogicComponentCpp, className, namespaceName));
            }

            bool result = false;
            for (int i = 0; i < 3 && !result; i++)
            {
                result = VisualStudioHelper.AddFilesToSolution(solution, projectName, new string[] { headerPath, cppPath });
                if (result == false && i < 2)
                {
                    System.Threading.Thread.Sleep(1000); //Wait a second before trying again (but only if we *are* going to try again)
                }
                else
                {
                    break;
                }
            }

            if (result == false)
            {
                Logger.Log(MessageType.Error, "Could not add files to solution!");
            }
        }
    }
}
