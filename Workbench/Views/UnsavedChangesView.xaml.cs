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
using System.Windows.Shapes;

namespace Workbench
{
    /// <summary>
    /// Interaction logic for UnsavedChangesView.xaml
    /// </summary>
    public partial class UnsavedChangesView : Window
    {
        public UnsavedChangesView()
        {
            InitializeComponent();
        }

        private void OnSave_Button_Click(object sender, RoutedEventArgs e)
        {
            Debug.Assert(ProjectVM.Current != null);

            try
            {
                ProjectVM.Save(ProjectVM.Current);
                DialogResult = true;
                Close();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                Logger.Log(MessageType.Error, "An error occures while trying to save the project!");
                MessageBox.Show("An error occures while trying to save the project! Error: " + ex.Message, "Error");
            }
        }

        private void OnDontSave_Button_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
            Close();
        }
    }
}
