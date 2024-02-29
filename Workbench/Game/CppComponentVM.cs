using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

using Workbench.GadgetAPIStructs;

namespace Workbench
{
	[DataContract]
    public class CppComponentVM : ComponentVM
    {
		private List<GadgetAPIStructs.NamedVar> _properties;
		private string _typeName;
		private ulong _guid;
		private bool _isActivated;

        [DataMember]
		public List<GadgetAPIStructs.NamedVar> Properties
        {
			get => _properties;
            set
            {
				if (_properties != value)
				{
					_properties = value;
					OnPropertyChanged(nameof(Properties));
                }
            }
        }

		[DataMember]
		public string TypeName
        {
			get => _typeName;
			set
			{
				if (_typeName != value)
				{
					_typeName = value;
					OnPropertyChanged(nameof(TypeName));
				}
			}
        }

		[DataMember]
		public ulong GUID
		{
			get => _guid;
			set
			{
				if (_guid != value)
				{
					_guid = value;
					OnPropertyChanged(nameof(GUID));
				}
			}
		}

		[DataMember]
		public bool IsActivated
		{
			get => _isActivated;
			set
			{
				if (_isActivated != value)
				{
					_isActivated = value;
					OnPropertyChanged(nameof(IsActivated));
				}
			}
		}

		public CppComponentVM(GameObjectVM owner, string type, ulong guid, bool isActivated, List<NamedVar> properties) : base(owner)
		{
			_typeName = type;
			_guid = guid;
			_isActivated = isActivated;
			_properties = properties;
		}

        public override IMultiSelectComponent GetMultiSelectComponent(MultiSelectedObjectVM msGo) => new MultiSelectCppComponentVM(msGo);
    }

    public sealed class MultiSelectCppComponentVM : MultiSelectComponentVM<CppComponentVM>
    {
        public MultiSelectCppComponentVM(MultiSelectedObjectVM msObj) : base(msObj)
        {
            Refresh();
        }

        protected override bool UpdateComponents(string propertyName)
        {
            return false;
        }

        protected override bool UpdateMultiSelectComponent()
        {
            return true;
        }
    }
}
