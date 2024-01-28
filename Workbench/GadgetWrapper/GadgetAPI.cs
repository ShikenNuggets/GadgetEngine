using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

using Workbench.GadgetAPIStructs;

namespace Workbench.GadgetAPIStructs
{
	[StructLayout(LayoutKind.Sequential)]
    class TransformInfo
	{
		public Vector3 position;
		public Vector3 rotation;
		public Vector3 scale;
	};

	[StructLayout(LayoutKind.Sequential)]
	class GameObjectDescriptor
	{
		public string name = "GameObject";
		public TransformInfo transform = new TransformInfo();
	};
}

namespace Workbench
{
    static class GadgetAPI
    {
        private const string _dllName = "Gadget.dll";

		[DllImport(_dllName, CharSet = CharSet.Ansi)]
		public static extern int LoadGameCodeDLL(string dllPath);
        [DllImport(_dllName)]
        public static extern int UnloadGameCodeDLL();

		[DllImport(_dllName)]
		public static extern bool InitForWorkbench();
        [DllImport(_dllName, CharSet = CharSet.Ansi)]
		private static extern ulong CreateGameObject(GameObjectDescriptor descriptor_);
		[DllImport(_dllName)]
		private static extern void DestroyGameObject(ulong guid_);

		public static ulong CreateGameObject(GameObjectVM gameObject)
		{
			GameObjectDescriptor desc = new GameObjectDescriptor();

			if (gameObject.Name != null)
			{
                desc.name = gameObject.Name;
            }

            //Transform
            var transform = gameObject.GetComponent<TransformComponentVM>();
			//Debug.Assert(transform != null); //TODO - This assert is currently meaningless, but we'll want it back later
			if (transform == null)
			{
				Logger.Log(MessageType.Warning, "All GameObjects should have a Transform component!");
			}
			else
			{
				desc.transform.position = transform.Position;
				desc.transform.rotation = transform.Rotation;
				desc.transform.scale = transform.Scale;
			}

			try
			{
				return CreateGameObject(desc);
			}
			catch (Exception ex)
            {
				Logger.Log(MessageType.Error, $"An error occured while calling external code from {_dllName}: " + ex.Message);
				MessageBox.Show($"An error occured while calling external code from {_dllName}: " + ex.Message);
				throw;
			}
		}

		public static void DestroyGameObject(GameObjectVM gameObject)
		{
			try
			{
                DestroyGameObject(gameObject.GUID);
            }
            catch (Exception ex)
            {
                Logger.Log(MessageType.Error, $"An error occured while calling external code from {_dllName}: " + ex.Message);
                MessageBox.Show($"An error occured while calling external code from {_dllName}: " + ex.Message);
                throw;
            }
        }
    }
}