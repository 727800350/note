class Animal {
	int health = 100;
	public void print(){System.out.println("Animal");}
	public void f_animal(){System.out.println("f_animal");}
}

class Mammal extends Animal {
	public void print(){System.out.println("Mammal");}
	public void f_mammal(){System.out.println("f_mammal");}
}

class Cat extends Mammal {
	public void print(){System.out.println("Cat");}
	public void f_cat(){System.out.println("f_cat");}
}

class Dog extends Mammal {
	public void print(){System.out.println("Dog");}
	public void f_dog(){System.out.println("f_dog");}
}
