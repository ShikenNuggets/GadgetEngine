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
using System.Windows.Media.Animation;
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

                newProjectArea.IsEnabled = false;
                openProjectArea.IsEnabled = true;
                AnimateToOpenProject();
            }else if(sender == newProjectButton){
                openProjectButton.IsChecked = false;
                newProjectButton.IsChecked = true;

                openProjectArea.IsEnabled = false;
                newProjectArea.IsEnabled = true;
                AnimateToNewProject();
            }
        }

        private readonly CubicEase _easing = new CubicEase() { EasingMode = EasingMode.EaseInOut };

        private void AnimateToOpenProject(double animLength = 0.5)
        {
            var highlightAnimation = new DoubleAnimation(460, 220, new Duration(TimeSpan.FromSeconds(animLength)));
            highlightAnimation.EasingFunction = _easing;
            highlightRect.BeginAnimation(Canvas.LeftProperty, highlightAnimation);

            openProjectArea.Visibility = Visibility.Visible;
            var animation = new ThicknessAnimation(new Thickness(-800, 0, 0, 0), new Thickness(0), new Duration(TimeSpan.FromSeconds(animLength)));
            animation.EasingFunction = _easing;
            browserContent.BeginAnimation(MarginProperty, animation);
        }

        private void AnimateToNewProject(double animLength = 0.5)
        {
            var highlightAnimation = new DoubleAnimation(220, 460, new Duration(TimeSpan.FromSeconds(animLength)));
            highlightAnimation.EasingFunction = _easing;
            highlightRect.BeginAnimation(Canvas.LeftProperty, highlightAnimation);

            var animation = new ThicknessAnimation(new Thickness(0), new Thickness(-800, 0, 0, 0), new Duration(TimeSpan.FromSeconds(animLength)));
            animation.EasingFunction = _easing;
            browserContent.BeginAnimation(MarginProperty, animation);
            animation.Completed += (s, e) =>
            {
                openProjectArea.Visibility = Visibility.Collapsed;
            };
        }
    }
}
