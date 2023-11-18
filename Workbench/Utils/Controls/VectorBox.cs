using System;
using System.Windows;
using System.Windows.Controls;

namespace Workbench.Controls
{
    public enum VectorType
    {
        Vector2,
        Vector3,
        Vector4
    }

    class VectorBox : Control
    {
        public VectorType VectorType
        {
            get => (VectorType)GetValue(VectorTypeProperty);
            set => SetValue(VectorTypeProperty, value);
        }

        public string XValue
        {
            get => (string)GetValue(XValueProperty);
            set => SetValue(XValueProperty, value);
        }

        public string YValue
        {
            get => (string)GetValue(YValueProperty);
            set => SetValue(YValueProperty, value);
        }

        public string ZValue
        {
            get => (string)GetValue(ZValueProperty);
            set => SetValue(ZValueProperty, value);
        }

        public string WValue
        {
            get => (string)GetValue(WValueProperty);
            set => SetValue(WValueProperty, value);
        }

        public double Multiplier
        {
            get => (double)GetValue(MultiplierProperty);
            set => SetValue(MultiplierProperty, value);
        }

        public Orientation Orientation
        {
            get => (Orientation)GetValue(OrientationProperty);
            set => SetValue(OrientationProperty, value);
        }

        public static readonly DependencyProperty VectorTypeProperty = DependencyProperty.Register(nameof(VectorType), typeof(VectorType), typeof(VectorBox), new FrameworkPropertyMetadata(VectorType.Vector3, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
        public static readonly DependencyProperty XValueProperty = DependencyProperty.Register(nameof(XValue), typeof(string), typeof(VectorBox), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
        public static readonly DependencyProperty YValueProperty = DependencyProperty.Register(nameof(YValue), typeof(string), typeof(VectorBox), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
        public static readonly DependencyProperty ZValueProperty = DependencyProperty.Register(nameof(ZValue), typeof(string), typeof(VectorBox), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
        public static readonly DependencyProperty WValueProperty = DependencyProperty.Register(nameof(WValue), typeof(string), typeof(VectorBox), new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
        public static readonly DependencyProperty MultiplierProperty = DependencyProperty.Register(nameof(Multiplier), typeof(double), typeof(VectorBox), new FrameworkPropertyMetadata(1.0, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
        public static readonly DependencyProperty OrientationProperty = DependencyProperty.Register(nameof(Orientation), typeof(Orientation), typeof(VectorBox), new FrameworkPropertyMetadata(Orientation.Horizontal, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));

        static VectorBox()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(VectorBox), new FrameworkPropertyMetadata(typeof(VectorBox)));
        }
    }
}
