using System;
using System.Collections.Generic;
using System.Collections.Specialized;
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

namespace Workbench.Editors
{
    /// <summary>
    /// Interaction logic for SceneEditorView.xaml
    /// </summary>
    public partial class SceneEditorView : UserControl
    {
        public SceneEditorView()
        {
            InitializeComponent();
            Loaded += OnSceneEditorViewLoaded;
        }

        private void OnSceneEditorViewLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnSceneEditorViewLoaded;
            Focus();
        }

        private void OnNewClass_Button_Click(object sender, RoutedEventArgs e)
        {
            new AddCppDialog().ShowDialog();
        }
    }
}
