#ifndef GADGET_APP_H

namespace Gadget{
	class App{
	public:
		static App* GetInstance();

		#ifdef _DEBUG
		static void DeleteInstance(); //Only use this for testing proper shutdown, don't use this in production
		#endif //_DEBUG

		void Run();

	private:
		static App* instance;

		App(){}
		~App(){}

		void Initialize();
	};
}

#endif //!GADGET_APP_H