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
    }
}