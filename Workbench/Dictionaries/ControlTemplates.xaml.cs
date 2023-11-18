using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Workbench.Dictionaries
{
    public partial class ControlTemplates : ResourceDictionary
    {
        private void OnTextBox_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            Debug.Assert(sender != null);
            Debug.Assert(e != null);
            Debug.Assert(sender is TextBox);

            var textBox = sender as TextBox;
            Debug.Assert(textBox != null);

            var exp = textBox.GetBindingExpression(TextBox.TextProperty);
            if (exp == null)
            {
                return;
            }

            if (e.Key == System.Windows.Input.Key.Enter)
            {
                if (textBox.Tag is ICommand command && command.CanExecute(textBox.Text))
                {
                    command.Execute(textBox.Text);
                }
                else
                {
                    exp.UpdateSource();
                }

                Keyboard.ClearFocus();
                e.Handled = true;
            }
            else if (e.Key == System.Windows.Input.Key.Escape)
            {
                exp.UpdateSource();
                Keyboard.ClearFocus();
            }
        }

        private void OnTextBoxRename_KeyDown(object sender, KeyEventArgs e)
        {
            Debug.Assert(sender != null);
            Debug.Assert(e != null);
            Debug.Assert(sender is TextBox);

            var textBox = sender as TextBox;
            Debug.Assert(textBox != null);

            var exp = textBox.GetBindingExpression(TextBox.TextProperty);
            if (exp == null)
            {
                return;
            }

            if (e.Key == System.Windows.Input.Key.Enter)
            {
                if (textBox.Tag is ICommand command && command.CanExecute(textBox.Text))
                {
                    command.Execute(textBox.Text);
                }
                else
                {
                    exp.UpdateSource();
                }

                textBox.Visibility = Visibility.Collapsed;
                e.Handled = true;
            }
            else if (e.Key == System.Windows.Input.Key.Escape)
            {
                exp.UpdateSource();
                textBox.Visibility = Visibility.Collapsed;
            }
        }

        private void OnTextBoxRename_LostFocus(object sender, RoutedEventArgs e)
        {
            Debug.Assert(sender != null);
            Debug.Assert(e != null);
            Debug.Assert(sender is TextBox);

            var textBox = sender as TextBox;
            Debug.Assert(textBox != null);

            if(!textBox.IsVisible)
            {
                return;
            }

            var exp = textBox.GetBindingExpression(TextBox.TextProperty);
            if (exp != null)
            {
                exp.UpdateTarget();
                textBox.Visibility = Visibility.Collapsed;
            }
        }

        private void OnClose_Button_Click(object sender, RoutedEventArgs e)
        {
            var window = (Window)((FrameworkElement)sender).TemplatedParent;
            window.Close();
        }

        private void OnMaximizeRestore_Button_Click(object sender, RoutedEventArgs e)
        {
            var window = (Window)((FrameworkElement)sender).TemplatedParent;
            if (window.WindowState == WindowState.Normal)
            {
                window.WindowState = WindowState.Maximized;
            }
            else
            {
                window.WindowState = WindowState.Normal;
            }
        }

        private void OnMinimize_Button_Click(object sender, RoutedEventArgs e)
        {
            var window = (Window)((FrameworkElement)sender).TemplatedParent;
            window.WindowState = WindowState.Minimized;
        }
    }
}