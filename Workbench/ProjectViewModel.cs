using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace Workbench
{
    [DataContract(Name = "Game")]
    public class ProjectViewModel : BaseViewModel
    {
        public static string Extension { get; } = ".wbn";

        [DataMember] public string Name { get; private set; }
        [DataMember] public string Path { get; private set; }

        public string FullPath => GetFullPath(Path, Name);

        [DataMember(Name = "Scenes")] private ObservableCollection<SceneViewModel> _scenes = new ObservableCollection<SceneViewModel>();
        public ReadOnlyObservableCollection<SceneViewModel> Scenes { get; }

        public ProjectViewModel(string name, string path)
        {
            Name = name;
            Path = path;

            _scenes.Add(new SceneViewModel(this, "DefaultScene"));
        }

        public static string GetFullPath(string path, string name)
        {
            return $"{path}\\{name}{Extension}";
        }
    }
}
