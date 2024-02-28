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

	[StructLayout(LayoutKind.Explicit)]
	public struct Var
	{
		[FieldOffset(0)] public ulong strVal;
		[FieldOffset(0)] public bool boolVal;
		[FieldOffset(0)] public double numVal;
	}

	[StructLayout(LayoutKind.Explicit)]
	public struct NamedVar
	{
		[FieldOffset(0)] public ulong name;
		[FieldOffset(8)] public Var value;
	}
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


		[DllImport(_dllName)]
		private static extern ulong GetNumDeclaredComponents();

		[DllImport(_dllName)]
		private static extern void GetDeclaredComponents(IntPtr handle);

		[DllImport(_dllName, CharSet = CharSet.Ansi)]
		private static extern ulong CreateComponentOfType(string type, ulong parentObjectGuid);

        [DllImport(_dllName)]
		private static extern ulong GetStringLengthFromID(ulong stringId);

        [DllImport(_dllName, CharSet = CharSet.Ansi)]
        private static extern void GetStringFromID(ulong stringId, StringBuilder str, ulong length);

        //-------------------------------------------------------------------------------

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

		public static List<string> GetDeclaredComponents()
		{
			//Get StringIDs of all declared components
			ulong[] strIds = new ulong[GetNumDeclaredComponents()];
			GCHandle strHandle = GCHandle.Alloc(strIds, GCHandleType.Pinned);
			GetDeclaredComponents(strHandle.AddrOfPinnedObject());

            List<string> declaredComponents = new((int)GetNumDeclaredComponents());
            for (int i = 0; i < strIds.Length; i++)
			{
				//Get the actual string from the StringID
				int length = (int)GetStringLengthFromID(strIds[i]);
                StringBuilder sb = new(length + 1); //+ 1 isn't strictly necessary here but it gives the C++ side some breathing room
				GetStringFromID(strIds[i], sb, (ulong)sb.Capacity);
				declaredComponents.Add(sb.ToString(0, length).Trim());
			}

			strHandle.Free(); //Don't forget to let the garbage collector know you're done!
			return declaredComponents;
		}

		public static void AddNewComponentToGameObject(GameObjectVM gameObject, string componentType)
		{
			if (gameObject == null)
			{
				return;
			}

			if (!GetDeclaredComponents().Contains(componentType))
			{
				return;
			}

			CreateComponentOfType(componentType, gameObject.GUID);
		}

		public static void AddNewComponentToGameObjects(MultiSelectedGameObjectVM gameObjects, string componentType)
		{
			if (gameObjects == null || gameObjects.SelectedObjects == null || gameObjects.SelectedObjects.Count == 0)
			{
				return;
			}

			foreach (GameObjectVM gameObject in gameObjects.SelectedObjects)
			{
				AddNewComponentToGameObject(gameObject, componentType);
			}
		}
    }
}