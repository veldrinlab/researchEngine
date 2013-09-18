/**
 * File contains declaration of templated EngineFramework based on Singleton pattern interface class.
 * @file    EngineFramework.hpp
 * @author  Szymon "Veldrin" Jab³oñski
 * @date    2012-05-23
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef ENGINEFRAMEWORK_HPP
#define ENGINEFRAMEWORK_HPP

namespace ResearchEngine
{
	/**
	 * Class represents implementaion of Singleton pattern. Is is templated in order to
	 * implement singleton pattern classes which will extend Singleton base class and 
	 * became singletons.
	 */
	template <typename T>
	class EngineFramework
	{
	protected:
		static T* instance;

		//timer
		float accumulator;
		int testAmount;
		int testAmountAccumulator;

		/**
		 * Class default constructor. Must be private and empty because of singleton pattern.
		 */
		EngineFramework() {}
			
		/**
		 * Class copy constructor. Must be private and empty because of singleton pattern.
		 * @param	s is copy source object.
		 */
		EngineFramework(const EngineFramework<T>& s) {}
			
		/**
		 * Class destructor
		 */
		virtual ~EngineFramework() {}

		/**
		 * Class assignment operator. Must be private and empty because of singleton pattern.
		 * @param	s is copy source object.
		 */  
		EngineFramework& operator=(const EngineFramework<T>& s){}

		virtual void update() {};
		virtual void render() {};
		virtual void reshape(int w, int h) {};
		virtual void keyboardInput(unsigned char key, int x, int y) {};
		virtual void mouseInput(int x, int y) {};

	public:
		/**
		 * Method is used to get singleton class instance. It use lazy evaluation.
		 * @return	instance of singleton object.
		 */
		static inline T* getInstance()
		{
			if (instance == nullptr)
				instance = new T;
			return instance;
		}

		/**
		 * Method is sused to delete allocated memory in lazy evaluation - if object was
		 * created.
		 */
		static inline void killInstance()
		{
			if (instance != nullptr)
				delete instance;
			instance = nullptr;
		}

		static void updateWrapper()
		{
			instance->update();
		}

		static void renderWrapper()
		{
			instance->render();

		}

		static void reshapeWrapper(int w, int h)
		{
			instance->reshape(w,h);
		}

		static void keyboardInputWrapper(unsigned char key, int x, int y)
		{
			instance->keyboardInput(key,x,y);
		}

		static void mouseInputWrapper(int x, int y)
		{
			instance->mouseInput(x,y);
		}
	};
		
	/**
	 * Initialization of class static member to null pointer.
	 */
	template<typename T> T* EngineFramework<T>::instance = nullptr;

}
#endif
