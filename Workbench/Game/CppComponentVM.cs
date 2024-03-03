using System;
using System.Collections.Generic;
using System.Diagnostics;
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
		private string _typeName;
		private ulong _guid;
		private bool _isActivated;
        private List<NamedVar> _properties;

		[DataMember]
		public string TypeName
        {
			get => _typeName;
			private set
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
			private set
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

        [DataMember]
        public List<NamedVar> Properties
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

        public CppComponentVM(GameObjectVM owner, string type, ulong guid, bool isActivated, List<NamedVar> properties) : base(owner)
		{
			_typeName = type;
			_guid = guid;
			_isActivated = isActivated;
			_properties = properties;
		}

        public override IMultiSelectComponent GetMultiSelectComponent(MultiSelectedObjectVM msGo, string? subType) => new MultiSelectCppComponentVM(msGo, subType);
    }

    public sealed class MultiSelectCppComponentVM : MultiSelectComponentVM<CppComponentVM>
    {
		private string? typeName;
		private ulong? guid;
		private bool? isActivated;
		private List<NamedVar> properties;

		public string? TypeName
		{
			get => typeName;
			private set
			{
				if (typeName != value)
				{
					typeName = value;
					OnPropertyChanged(nameof(TypeName));
				}
			}
		}

		public ulong? GUID
		{
			get => guid;
			private set
			{
				if (guid != value)
				{
					guid = value;
					OnPropertyChanged(nameof(GUID));
				}
			}
		}

		public bool? IsActivated
		{
			get => isActivated;
			set
			{
				if (isActivated != value)
				{
					isActivated = value;
					OnPropertyChanged(nameof(IsActivated));
				}
			}
		}

		public List<NamedVar> Properties
		{
			get => properties;
			set
			{
				if (properties  != value)
				{
					properties = value;
					OnPropertyChanged(nameof(Properties));
				}
			}
		}

        public MultiSelectCppComponentVM(MultiSelectedObjectVM msObj, string? subType) : base(msObj, subType)
        {
			Debug.Assert(msObj != null);
			Debug.Assert(!string.IsNullOrWhiteSpace(subType));
			properties = new List<NamedVar>();
            Refresh();
        }

        protected override bool UpdateComponents(string propertyName)
        {
			switch (propertyName)
			{
				case nameof(IsActivated):
                    SelectedComponents.ForEach(c => c.IsActivated = isActivated ?? c.IsActivated);
                    break;
				default:
					SelectedComponents.ForEach(c => {
						int i = Utils.Contains(c.Properties, propertyName);
						if (i >= 0 && c.Properties != null && properties != null)
						{
							if (properties[i].value.type != 0)
							{
								c.Properties[i] = new NamedVar(c.Properties[i].name, properties[i].value);
							}
						}
					});
					break;
			}

            return false;
        }

        protected override bool UpdateMultiSelectComponent()
        {
			TypeName = MultiSelectedObjectVM.GetMixedValues(SelectedComponents, new Func<CppComponentVM, string>(t => t.TypeName));
			GUID = MultiSelectedObjectVM.GetMixedValues(SelectedComponents, new Func<CppComponentVM, ulong>(t => t.GUID));
			IsActivated = MultiSelectedObjectVM.GetMixedValues(SelectedComponents, new Func<CppComponentVM, bool>(t => t.IsActivated));
			Properties = MultiSelectedObjectVM.GetMixedValues(SelectedComponents, new Func<CppComponentVM, List<NamedVar>>(t => t.Properties)); //TODO - Not sure how best to handle this

            return true;
        }
    }
}
