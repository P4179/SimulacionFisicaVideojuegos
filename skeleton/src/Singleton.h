#pragma once

using namespace std;

template<typename T>
// instancia unica
class Singleton {
private:
	static T* instance;

protected:
	Singleton() {}

public:
	// no se pueden copiar objetos de ese tipo puesto que hay una unica instancia
	Singleton<T>& operator=(const Singleton<T>& o) = delete;
	Singleton(const Singleton<T>& o) = delete;

	virtual ~Singleton() {}

	// inicializar el singleton
	// se hace de esta forma para que no se puede crear mas de una instancia
	template<typename ...Ts>
	inline static T* init(Ts &&...args) {
		if (instance == nullptr) {
			instance = new T(forward<Ts>(args)...);
		}
		return instance;
	}

	inline static T* get() {
		return instance;
	}
};

// se inicialza fuera de la clase al ser un atributo estatico
template<typename T>
T* Singleton<T>::instance = nullptr;