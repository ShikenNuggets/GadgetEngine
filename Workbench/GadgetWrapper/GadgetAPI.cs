using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Numerics;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

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
        public enum VarType
        {
			Null = 0,
			String = 1,
			Bool = 2,
			Number = 3
        }

		public Var(Var? var)
		{
			if (var.HasValue == false)
			{
				type = (int)VarType.Null;
				strVal = 0;
			}
			else
			{
				type = var.Value.type;
				strVal = var.Value.strVal;
			}
		}

        public Var(ulong str)
		{
			type = (int)VarType.String;
			strVal = str;
		}

        public Var(bool boolean)
		{
			type = (int)VarType.Bool;
			boolVal = boolean;
		}
		
		public Var(double number)
		{
			type= (int)VarType.Number;
			numVal = number;
		}

		public static bool operator ==(Var a, Var b)
		{
			return a.type == b.type && a.strVal == b.strVal;
		}

		public static bool operator !=(Var a, Var b)
		{
            return a.type != b.type || a.strVal != b.strVal;
        }

        [FieldOffset(0)] public int type;
		[FieldOffset(4)] public ulong strVal;
		[FieldOffset(4)] public bool boolVal;
		[FieldOffset(4)] public double numVal;
	}

	[StructLayout(LayoutKind.Explicit)]
	public struct NamedVar
	{
        public NamedVar(ulong name, Var var)
		{
			this.name = name;
			value = var;
		}

        public static bool operator ==(NamedVar a, NamedVar b)
        {
            return a.value == b.value;
        }

        public static bool operator !=(NamedVar a, NamedVar b)
        {
            return a.value != b.value;
        }

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

		[DllImport(_dllName)]
		private static extern ulong GetComponentTypeName(ulong componentGuid_, ulong parentGuid_);

		[DllImport(_dllName)]
		private static extern bool GetComponentIsActivated(ulong componentGuid_, ulong parentGuid_);

		[DllImport(_dllName)]
		private static extern ulong GetNumComponentProperties(ulong componentGuid_, ulong parentGuid_);

		[DllImport(_dllName)]
		private static extern void GetComponentProperties(ulong componentGuid_, ulong parentGuid_, IntPtr namedVars);

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

			try
			{
                GetDeclaredComponents(strHandle.AddrOfPinnedObject());
            }
            catch (Exception ex)
            {
                Logger.Log(MessageType.Error, $"An error occured while calling external code from {_dllName}: " + ex.Message);
                MessageBox.Show($"An error occured while calling external code from {_dllName}: " + ex.Message);
				throw;
            }

            List<string> declaredComponents = new((int)GetNumDeclaredComponents());
            for (int i = 0; i < strIds.Length; i++)
			{
				declaredComponents.Add(GetStringFromID(strIds[i]));
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

			//Only continue if the component type is declared as serializable
			if (!GetDeclaredComponents().Contains(componentType))
			{
				return;
			}

			//Create a new component and add it to the GameObject
			ulong componentGuid = Utils.InvalidGUID;
            try
            {
				componentGuid = CreateComponentOfType(componentType, gameObject.GUID);
			}
			catch (Exception ex)
			{
				Logger.Log(MessageType.Error, $"An error occured while calling external code from {_dllName}: " + ex.Message);
                MessageBox.Show($"An error occured while calling external code from {_dllName}: " + ex.Message);
				throw;
			}

			//Get the type name
			var nameId = GetComponentTypeName(componentGuid, gameObject.GUID);
			var nameStrLen = GetStringLengthFromID(nameId);
			StringBuilder sb = new((int)nameStrLen + 1);
			GetStringFromID(nameId, sb, nameStrLen);
			var nameStr = sb.ToString(0, (int)nameStrLen).Trim();

			//Get isActivated
			var isActivated = GetComponentIsActivated(componentGuid, gameObject.GUID);

			//Get properties
			var properties = GetComponentProperties(componentGuid, gameObject.GUID);

			//Assemble the CppComponentVM and add it to the GameObjectVM
			gameObject.AddComponent(new CppComponentVM(gameObject, nameStr, componentGuid, isActivated, properties));
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

		public static List<NamedVar> GetComponentProperties(ulong componentGuid, ulong parentGuid)
		{
			List<NamedVar> final = new List<NamedVar>();

			NamedVar[] namedVars = new NamedVar[GetNumComponentProperties(componentGuid, parentGuid)];
			GCHandle nvHandle = GCHandle.Alloc(namedVars, GCHandleType.Pinned);

			GetComponentProperties(componentGuid, parentGuid, nvHandle.AddrOfPinnedObject());

			nvHandle.Free();

			final.AddRange(namedVars);
			return final;
		}

		public static string GetStringFromID(ulong id)
		{
            int length = (int)GetStringLengthFromID(id);
            StringBuilder sb = new(length + 1); //+ 1 isn't strictly necessary here but it gives the C++ side some breathing room
            GetStringFromID(id, sb, (ulong)sb.Capacity);
            return sb.ToString(0, length).Trim();
        }
    }
}