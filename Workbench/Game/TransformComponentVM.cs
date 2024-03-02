using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace Workbench
{
    //Transforms aren't technically components in the engine, but to simplify things we'll treat them like components in the editor
    [DataContract]
    public class TransformComponentVM : ComponentVM
    {
        private Vector3 _position;
        private Vector3 _rotation; //In the editor, rotations will simply be represented as Euler angles
        private Vector3 _scale = new(1.0f, 1.0f, 1.0f);

        [DataMember]
        public Vector3 Position
        {
            get => _position;
            set
            {
                if (_position != value)
                {
                    _position = value;
                    OnPropertyChanged(nameof(Position));
                }
            }
        }

        [DataMember]
        public Vector3 Rotation
        {
            get => _rotation;
            set
            {
                if (_rotation != value)
                {
                    _rotation = value;
                    OnPropertyChanged(nameof(Rotation));
                }
            }
        }

        [DataMember]
        public Vector3 Scale
        {
            get => _scale;
            set
            {
                if (_scale != value)
                {
                    _scale = value;
                    OnPropertyChanged(nameof(Scale));
                }
            }
        }

        public TransformComponentVM(GameObjectVM owner) : base(owner){}

        public override IMultiSelectComponent GetMultiSelectComponent(MultiSelectedObjectVM msGo, string? _ = null) => new MultiSelectTransformVM(msGo);
    }

    public sealed class MultiSelectTransformVM : MultiSelectComponentVM<TransformComponentVM>
    {
        private float? _posX;
        private float? _posY;
        private float? _posZ;

        private float? _rotX;
        private float? _rotY;
        private float? _rotZ;

        private float? _scaleX;
        private float? _scaleY;
        private float? _scaleZ;

        public float? PosX
        {
            get => _posX;
            set
            {
                if (!Utils.Near(_posX, value))
                {
                    _posX = value;
                    OnPropertyChanged(nameof(PosX));
                }
            }
        }

        public float? PosY
        {
            get => _posY;
            set
            {
                if(!Utils.Near(_posY, value))
                {
                    _posY = value;
                    OnPropertyChanged(nameof(PosY));
                }
            }
        }

        public float? PosZ
        {
            get => _posZ;
            set
            {
                if(!Utils.Near(_posZ, value))
                {
                    _posZ = value;
                    OnPropertyChanged(nameof(PosZ));
                }
            }
        }

        public float? RotX
        {
            get => _rotX;
            set
            {
                if (!Utils.Near(_rotX, value))
                {
                    _rotX = value;
                    OnPropertyChanged(nameof(RotX));
                }
            }
        }

        public float? RotY
        {
            get => _rotY;
            set
            {
                if (!Utils.Near(_rotY, value))
                {
                    _rotY = value;
                    OnPropertyChanged(nameof(RotY));
                }
            }
        }

        public float? RotZ
        {
            get => _rotZ;
            set
            {
                if (!Utils.Near(_rotZ, value))
                {
                    _rotZ = value;
                    OnPropertyChanged(nameof(RotZ));
                }
            }
        }

        public float? ScaleX
        {
            get => _scaleX;
            set
            {
                if (!Utils.Near(_scaleX, value))
                {
                    _scaleX = value;
                    OnPropertyChanged(nameof(ScaleX));
                }
            }
        }

        public float? ScaleY
        {
            get => _scaleY;
            set
            {
                if (!Utils.Near(_scaleY, value))
                {
                    _scaleY = value;
                    OnPropertyChanged(nameof(ScaleY));
                }
            }
        }

        public float? ScaleZ
        {
            get => _scaleZ;
            set
            {
                if (!Utils.Near(_scaleZ, value))
                {
                    _scaleZ = value;
                    OnPropertyChanged(nameof(ScaleZ));
                }
            }
        }

        public MultiSelectTransformVM(MultiSelectedObjectVM obj) : base(obj, null)
        {
            Refresh();
        }

        protected override bool UpdateComponents(string propertyName)
        {
            switch (propertyName)
            {
                case nameof(PosX):
                case nameof(PosY):
                case nameof(PosZ):
                    SelectedComponents.ForEach(c => c.Position = new Vector3(_posX ?? c.Position.X, _posY ?? c.Position.Y, _posZ ?? c.Position.Z));
                    return true;
                case nameof(RotX):
                case nameof(RotY):
                case nameof(RotZ):
                    SelectedComponents.ForEach(c => c.Rotation = new Vector3(_posX ?? c.Rotation.X, _posY ?? c.Rotation.Y, _posZ ?? c.Rotation.Z));
                    return true;
                case nameof(ScaleX):
                case nameof(ScaleY):
                case nameof(ScaleZ):
                    SelectedComponents.ForEach(c => c.Scale = new Vector3(_posX ?? c.Scale.X, _posY ?? c.Scale.Y, _posZ ?? c.Scale.Z));
                    return true;
            }

            Debug.Assert(false);
            return false;
        }

        protected override bool UpdateMultiSelectComponent()
        {
            PosX = MultiSelectedObjectVM.GetMixedValues(SelectedComponents, new Func<TransformComponentVM, float>(t => t.Position.X));
            PosY = MultiSelectedObjectVM.GetMixedValues(SelectedComponents, new Func<TransformComponentVM, float>(t => t.Position.Y));
            PosZ = MultiSelectedObjectVM.GetMixedValues(SelectedComponents, new Func<TransformComponentVM, float>(t => t.Position.Z));

            RotX = MultiSelectedObjectVM.GetMixedValues(SelectedComponents, new Func<TransformComponentVM, float>(t => t.Rotation.X));
            RotY = MultiSelectedObjectVM.GetMixedValues(SelectedComponents, new Func<TransformComponentVM, float>(t => t.Rotation.Y));
            RotZ = MultiSelectedObjectVM.GetMixedValues(SelectedComponents, new Func<TransformComponentVM, float>(t => t.Rotation.Z));

            ScaleX = MultiSelectedObjectVM.GetMixedValues(SelectedComponents, new Func<TransformComponentVM, float>(t => t.Scale.X));
            ScaleY = MultiSelectedObjectVM.GetMixedValues(SelectedComponents, new Func<TransformComponentVM, float>(t => t.Scale.Y));
            ScaleZ = MultiSelectedObjectVM.GetMixedValues(SelectedComponents, new Func<TransformComponentVM, float>(t => t.Scale.Z));

            return true;
        }
    }
}
