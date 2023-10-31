using System;
using System.Collections.Generic;
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
        private Vector3 _scale;

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

        public TransformComponentVM(GameObjectVM owner) : base(owner)
        {
        }
    }
}
