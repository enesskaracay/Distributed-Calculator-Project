# Distributed-Calculator-Project
We are making a calculator in c language. In this calculator, we set up a data flow pipe structure with a main process and multiple sub-processes. A makefile was also created in the project.
# Türkçe
C dilinde bir hesap makinesi yapıyoruz. Bu hesap makinesinde, ana süreç ve birden fazla alt süreçle bir veri akış boru hattı yapısı kurduk. Ayrıca projede bir Makefile oluşturuldu.

![Screenshot2](https://github.com/user-attachments/assets/0675c69c-ce21-4c84-a8b6-4966e64ec3f4)
![Screenshot1](https://github.com/user-attachments/assets/de61dec5-4a76-4945-9695-2c72e4c6a2a1)
![Screenshot4](https://github.com/user-attachments/assets/aec26c65-11a0-41f3-8142-cf2b7e8d91b3)
![Screenshot3](https://github.com/user-attachments/assets/f3384cb1-a549-4c8c-b3de-421c7e12d243)
# Türkçe
# İş Akışı
* Hesap Makinesi programı başlar ve her işlem için dört alt süreç oluşturur.
* Kullanıcı, yapmak istediği işlemi ve iki operandı girer.  
* Ana program, girdiyi ilgili alt sürece özel bir boru hattı (pipe) üzerinden gönderir.  
* Alt süreç, hesaplamayı yapar ve sonucu ana programa geri gönderir.  
* Alt süreç ayrıca saver programını çağırır ve saver, sonucu **results.txt** dosyasına kaydeder.  
* Ana program sonucu ekrana yazdırır.  
* Ana program sona erdiğinde alt süreçler de kapanır.  
* Hesap Makinesi, kullanıcı "exit" seçeneğini seçene kadar çalışmaya devam eder.  

**Ana Program Menüsü:**  
▪ **Hesap Makinesi**  
▪ 1- Toplama  
▪ 2- Çıkarma  
▪ 3- Çarpma  
▪ 4- Bölme  
▪ 5- Çıkış

# Teknik Detaylar
* Her süreç, ana program ile iletişim için kendi özel boru hattını (pipe) kullanır. 
* Ana program, alt süreçler oluşturmak için fork kullanır ve alt süreçleri çalıştırmak için exec kullanır.
* Veri, ana program ile alt süreçler arasında boru hatları (pipes) kullanılarak aktarılır, saver hariç.
* wait sistem çağrısı, gerektiğinde ana programın alt süreçlerin tamamlanmasını beklemesini sağlar.
* Saver programı boru hattı kullanmaz. Sonuçları parametre olarak almanız gerekir.
* Proje içinde bir Makefile dosyası bulunmaktadır. Makefile çalıştırıldığında, programlar (C dosyaları) otomatik olarak derlenir. Ödev denetleyicisi yalnızca Makefile'ı çalıştıracak ve ardından terminalden myshell programını çalıştıracaktır.


# English
# Workflow
* The Calculator program starts and creates four subprocesses for each operation.
* The user inputs the desired operation and two operands.
* The main program sends the input to the appropriate subprocess through a dedicated pipe.
* The subprocess performs the calculation and sends the result back to the main program.
* The subprocess also will call saver and saver will save the result to results.txt file.
* The main program displays the result.
* The subprocess will exited main program end.
* Calculator will continue until “exit” selected. The menu of main program:
▪ Calculator
* 1- addition
* 2- subtraction
* 3- multiplication
* 4- division
* 5- exit

# Technical Details
* Each process uses its dedicated pipe for communication with the main program.
* The main program uses fork to create child processes and exec to execute the subprocesses.
* Data is transferred between the main program and subprocesses using pipes except saver.
* The wait system call ensures that the main program waits for subprocesses to complete where necessary.
* The saver program doesn’t use a pipe. You must pass results as parameters.
* A Makefile is included in the project. When the Makefile is executed, the programs (C files) will be compiled automatically. The homework checker will just run the Makefile and then run the myshell program from the terminal.


