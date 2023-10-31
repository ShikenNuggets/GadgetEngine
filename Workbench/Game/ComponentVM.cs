using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace Workbench
{
    [DataContract]
    public class ComponentVM : BaseViewModel
    {
        [DataMember] public GameObjectVM Owner { get; private set; }

        public ComponentVM(GameObjectVM owner)
        {
            Debug.Assert(owner != null);
            Owner = owner;
        }
    }
}
