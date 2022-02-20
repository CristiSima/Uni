class Complex {
	private int real, imaginary;

	public Complex(int real, int imaginary) {
		this.real = real;
		this.imaginary = imaginary;
	}
	public Complex() {
		this.real = this.imaginary = 0;
	}
	public Complex(Complex original) {
		this.real = original.real;
		this.imaginary = original.imaginary;
	}

	public int getReal() {
		return this.real;
	}
	public int getImaginary() {
		return this.imaginary;
	}
	public void setReal(int real) {
		this.real = real;
	}
	public void setImaginary(int imaginary) {
		this.imaginary = imaginary;
	}

	public void addWithComplex(Complex other) {
		this.real += other.getReal();
		this.imaginary += other.getImaginary();
	}

	public void showNumber() {
		System.out.println("" + this.real + "+" + this.imaginary + "i");
	}

	public static void main(String[] args) {
		Complex a = new Complex();
		Complex b = new Complex(2, 4);
		Complex c = new Complex(b);

		a.addWithComplex(c);
		b.addWithComplex(c);

		a.showNumber();
		b.showNumber();
		c.showNumber();
	}
}
