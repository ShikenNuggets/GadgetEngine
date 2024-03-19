using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Workbench.Controls
{
    [TemplatePart(Name ="PART_textBlock", Type = typeof(TextBlock))]
    [TemplatePart(Name ="PART_textBox", Type = typeof(TextBox))]
    class NumberBox : Control
    {
        private double _originalValue;
        private double _mouseXStart;
        private readonly double _defaultMultiplier = 0.01;
        private bool _mouseCaptured = false;
        private bool _valueChanged = false;

        public string? Value
        {
            get => (string)GetValue(ValueProperty);
            set => SetValue(ValueProperty, value);
        }

        public double Multiplier
        {
            get => (double)GetValue(MultiplierProperty);
            set => SetValue(MultiplierProperty, value);
        }

        public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(nameof(Value), typeof(string), typeof(NumberBox), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
        public static readonly DependencyProperty MultiplierProperty = DependencyProperty.Register(nameof(Multiplier), typeof(double), typeof(NumberBox), new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));

        static NumberBox()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(NumberBox), new FrameworkPropertyMetadata(typeof(NumberBox)));
        }

        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();
            if (GetTemplateChild("PART_textBlock") is TextBlock textBlock)
            {
                textBlock.MouseLeftButtonDown += OnTextBlock_MouseLeftButton_Down;
                textBlock.MouseLeftButtonUp += OnTextBlock_MouseLeftButton_Up;
                textBlock.MouseMove += OnTextBlock_Mouse_Move;
            }
        }

        private void OnTextBlock_MouseLeftButton_Down(object sender, MouseButtonEventArgs e)
        {
            double.TryParse(Value, out _originalValue);

            Mouse.Capture(sender as UIElement);
            _mouseCaptured = true;
            _valueChanged = false;
            _mouseXStart = e.GetPosition(this).X;

            Focus();

            e.Handled = true;
        }

        private void OnTextBlock_MouseLeftButton_Up(object sender, MouseButtonEventArgs e)
        {
            if (_mouseCaptured)
            {
                Mouse.Capture(null);
                _mouseCaptured = false;


                if (!_valueChanged && GetTemplateChild("PART_textBox") is TextBox textBox)
                {
                    textBox.Visibility = Visibility.Visible;
                    textBox.Focus();
                    textBox.SelectAll();
                }

                e.Handled = true;
            }
        }

        private void OnTextBlock_Mouse_Move(object sender, MouseEventArgs e)
        {
            if (_mouseCaptured)
            {
                var mouseX = e.GetPosition(this).X;
                var delta = mouseX - _mouseXStart;
                if (Math.Abs(delta) > SystemParameters.MinimumHorizontalDragDistance)
                {
                    double multiplier = _defaultMultiplier * Multiplier;
                    if (Keyboard.Modifiers.HasFlag(ModifierKeys.Control))
                    {
                        multiplier /= 10;
                    }
                    else if (Keyboard.Modifiers.HasFlag(ModifierKeys.Shift))
                    {
                        multiplier *= 10;
                    }

                    var newValue = _originalValue + (delta * multiplier);
                    Value = newValue.ToString("0.#####");
                    _valueChanged = true;
                }
            }
        }
    }
}