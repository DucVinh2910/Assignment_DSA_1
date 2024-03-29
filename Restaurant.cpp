#include "main.h"

class imp_res : public Restaurant
{
	class TIME;
private:
	int sizeInDesk; // số lượng khách hàng trong bàn ăn
	int sizeInQueue; // số lượng khách hàng trong hàng chờ
	customer* Desk; // lưu vị trí của khách hàng vừa cập nhật trong bàn ăn
	customer* Queue; // lưu vị trí đầu tiên của khách hàng trong queue
	TIME* CusTimeHead; // khách hàng đầu tiên tới nhà hàng
	TIME* CusTimeTail; // khách hàng cuối cùng tới nhà hàng

public:	
	imp_res() {
		sizeInQueue = sizeInDesk = 0;
		Desk = Queue =  nullptr;
		CusTimeHead = CusTimeTail = nullptr;
	};
	~imp_res(){
		while(CusTimeHead){
			TIME* temp = CusTimeHead;	
			CusTimeHead = CusTimeHead->next;
			delete temp;
		}
	}
	void RED(string name, int energy);
	void BLUE(int num);
	void PURPLE();
	void REVERSAL();
	void DOMAIN_EXPANSION();
	void UNLIMITED_VOID();
	void LIGHT(int num);
private:
	class TIME{
	public:
		customer* data; // lưu toàn bộ thông tin của customer
		bool Desk_true_Queue_false; // biến dùng để check xem có nằm ở trong bàn ăn không
		TIME* next; // con trỏ next
		TIME* prev; // con trỏ prev
	public:

		TIME(customer* data, bool Desk_true_Queue_false, TIME* next = nullptr, TIME* prev = nullptr)
		:data(data),next(next),prev(prev), Desk_true_Queue_false(Desk_true_Queue_false) {}

		~TIME(){
			delete data;
		}
	};

	// RED
	void insertCustomerToDesk(string name, int energy); // HÀM DÙNG ĐỂ THÊM KHÁCH HÀNG VÀO BÀN ĂN 

	// BLUE
	void deleteCustomerInDesk(TIME*); // HÀM DÙNG ĐỂ XÓA KHÁCH HÀNG KHỎI BÀN ĂN
	void insertCustomerQueueToInDesk(); // HÀM DÙNG ĐỂ THÊM KHÁCH HÀNG TỪ HÀNG ĐỢI VÀO BÀN ĂN
	void deleteCustomerByTime(TIME*); // HÀM NÀY DÙNG ĐỂ XÓA KHÁCH THEO THỜI GIAN
	// DOMAIN_EXPANSION
	void deleteCustomerInQueue(TIME*); // HÀM DÙNG ĐỂ XÓA KHÁCH HÀNG TRONG HÀNG ĐỢI

	// REVERSAL
	customer* WizardHead(customer* x); // HÀM TRẢ VỀ HEAD CỦA CHÚ THUẬT SƯ
	customer* WizardTail(customer* x); // HÀM TRẢ VỀ TAIL CỦA CHÚ THUẬT SƯ
	customer* SpiritHead(customer* x); // HÀM TRẢ VỀ HEAD CỦA OÁN LINH
	customer* SpiritTail(customer* x); // HÀM TRẢ VỀ TAIL CỦA OÁN LINH
	void Wizard(customer* x); // HÀM DÙNG ĐỂ ĐẢO NGƯỢC HEAD VÀ TAIL CỦA CHÚ THUẬT SƯ
	void Spirit(customer* x); // HÀM DÙNG ĐỂ ĐẢO NGƯỢC HEAD VÀ TAIL CỦA OÁN LINH
	void swapAddress(customer* head, customer * tail); // HÀM DÙNG ĐỂ SWAP ĐỊA CHỈ

	// PURPLE
	int sort(customer* head, int n, int increment); // HÀM NÀY DÙNG ĐỂ TRẢ VỀ SỐ LƯỢNG CẦN XÓA VÀ SORT THEO SHELL SORT
	customer* getCustomer(customer* x, int idx); // HÀM NÀY DÙNG ĐỂ TÌM VỊ TRÍ CỦA CUSTOMER DỰA VÀO INDEX
	bool checkTime(customer* a, customer* b); // HÀM NÀY DÙNG ĐỂ CHECK AI ĐẾN TRƯỚC 
};

// B1: Các trường hợp đuổi khách
// B1.1: Energy = 0
// B1.2: Hết chỗ -> sizeDesk + sizeQueue >= MAXSIZE * 2
// B1.3: Trùng tên với khách đã có mặt trong bàn ăn hoặc hàng chờ
// B2: SizeDesk = 0 và sizeDesk = MAXSIZE
// B2.1: Trường hợp sizeDesk = 0 thì thêm vào đầu
// B2.2: Trường hợp sizeDesk = MAXSIZE thì thêm khách vào hàng chờ
// B2.2.1: Nếu sizeQueue = 0 thì thêm đầu
// B2.2.2: Nếu sizeQueue != 0 
// B2.2.3: Cập nhật biến thời gian
// B3: Trường hợp sizeDesk >= MAXSIZE/2 -> sắp xếp theo chiều kim đồng hồ hoặc ngược chiều kim đồng hồ
// B3.1: Kiểm tra khách hàng mới vào là energy < 0 hay > 0 để sắp xếp và đưa customerX tới điểm cần xếp theo trị tuyệt đối lớn nhất
// B4: Sắp xếp khách hàng theo chiều kim đồng hồ hoặc ngược kim đồng hồ
// B4.1: Nếu khách hàng đó có energy > 0 -> theo chiều kim đồng hồ
// B4.2: Nếu khách hàng đó có energy < 0 -> theo ngược chiều kim đồng hồ
// B4.3: Cập nhật lại khách hàng mới vào là customerX
// B4.4: Cập nhật biến thời gian
void imp_res::insertCustomerToDesk(string name, int energy){
	// B2.1: Trường hợp sizeDesk = 0 -> thêm vào đầu bàn ăn
	if(sizeInDesk == 0){
		// tạo mới 1 khách hàng
		Desk = new customer(name, energy, nullptr, nullptr); 
		// cập nhật biến next prev
		Desk->next = Desk->prev = Desk;
		// cập nhật size
		sizeInDesk ++;
		// B2.1.1: Cập nhật biến thời gian
		CusTimeTail = CusTimeHead = new TIME(Desk, true); 
		return;
	}
	// B2.2: SizeDesk = 0 và sizeDesk = MAXSIZE
	else if(sizeInDesk == MAXSIZE){
		// Tạo nếu customer
		customer* newCustomerQueue = new customer(name, energy, nullptr, nullptr);
		// B2.2.1: Nếu sizeQueue = 0 thì thêm đầu 
		if(sizeInQueue == 0){
			Queue = newCustomerQueue;
			Queue->next = Queue->prev = newCustomerQueue;
		}
		// B2.2.2: Nếu sizeQueue != 0 
		else{
			newCustomerQueue->next = Queue; //! cập nhật thằng mới vô
			newCustomerQueue->prev = Queue->prev; //! cập nhật thằng mới vô
			Queue->prev = newCustomerQueue->prev->next = newCustomerQueue;		
		}
		// cập nhật size
		sizeInQueue ++;


		// B2.2: Cập nhật biến thời gian
		TIME* newCustomerTime = new TIME(newCustomerQueue, false); 
		CusTimeTail->next = newCustomerTime;
		newCustomerTime->prev = CusTimeTail;
		CusTimeTail = newCustomerTime;
		return;
	}
	

	// B3: Trường hợp sizeDesk >= MAXSIZE/2 -> sắp xếp theo chiều kim đồng hồ hoặc ngược chiều kim đồng hồ
	if(sizeInDesk >= MAXSIZE / 2){
		// tạo 1 biến để bắt đầu kiểm tra
		customer* temp = Desk;
		// duyệt vòng for để xem abs nào là lớn nhất và cập nhật customerX tới đó để bắt đầu xếp
		for(int i = 0; i < sizeInDesk; i++){
			if(abs(energy - temp->energy) > abs(energy - Desk->energy)) Desk = temp;		
			temp = temp->next; 
		}
	}

	
	// B4: Nếu khách hàng đó có energy > 0 -> theo chiều kim đồng hồ
	customer* newCustomerInDesk = new customer(name, energy, nullptr, nullptr); 
	if(energy >= Desk->energy){
		newCustomerInDesk->next = Desk->next;
		newCustomerInDesk->prev = Desk;
		Desk->next = newCustomerInDesk->next->prev = newCustomerInDesk;
	}
	// B4.2: Nếu khách hàng đó có energy < 0 -> theo ngược chiều kim đồng hồ
	else{
		newCustomerInDesk->next = Desk;
		newCustomerInDesk->prev = Desk->prev;
		Desk->prev = newCustomerInDesk->prev->next = newCustomerInDesk;
	}

	// B4.3: Cập nhật lại khách hàng mới vào là customerX
	Desk = newCustomerInDesk;
	// Cập nhật size
	sizeInDesk ++;

	// B4.4: Cập nhật biến thời gian
	TIME* newCustomerTime = new TIME(Desk, true);
	CusTimeTail->next = newCustomerTime;
	newCustomerTime->prev = CusTimeTail;
	CusTimeTail = newCustomerTime;
}
void imp_res::RED(string name, int energy){
	// B1: Các trường hợp đuổi khách
	// B1.1: energy = 0
	if(energy == 0) return;
	// B1.2: hết chỗ -> sizeDesk + sizeQueue >= MAXSIZE * 2
	else if(sizeInDesk + sizeInQueue >= MAXSIZE * 2) return;
	// B1.3: trùng tên với khách đã có mặt trong bàn ăn hoặc hàng chờ
	else{
		TIME* temp = CusTimeHead;
		//* xem thử khác hàng có chung tên không với khách nào hay không
		while(temp != nullptr){	
			//TODO tìm thử trung tên không
			if(temp->data->name == name) return;
			temp = temp->next;
		}	
	}
	// Thêm khách vào bàn
	insertCustomerToDesk(name, energy);
}

// HÀM NÀY DÙNG ĐỂ ĐUỔI KHÁCH Ở TRONG BÀN ĂN
// B1: Trong bàn ăn có 1 khách
// B2: Đuổi khách khi trong bàn ăn có >= 2
// B2.1: Nếu khách có energy > 0 thì cập nhật customerX là ->next: theo chiều kim đồng hồ
// B2.2: Nếu khách có energy < 0 thì cập nhật customerX là ->prev: theo ngược chiều kim đồng hồ
void imp_res::deleteCustomerInDesk(TIME* customerTimeDelete){
	// B1: Trong bàn ăn có 1 khách
	if(sizeInDesk == 1) Desk = nullptr;
	// B2: Đuổi khách khi trong bàn ăn có >= 2
	else{
		// biến cần delete
		customer* customerDelete = customerTimeDelete->data;

		// cập nhật trước sau
		customerDelete->next->prev = customerDelete->prev;
		customerDelete->prev->next = customerDelete->next;

		// B2.1: Nếu khách có energy > 0 thì cập nhật customerX là ->next: theo chiều kim đồng hồ
		if(customerDelete->energy > 0) Desk = customerDelete->next;
		// B2.2: Nếu khách có energy < 0 thì cập nhật customerX là ->prev: theo ngược chiều kim đồng hồ
		else Desk = customerDelete->prev;
	}
	// cập nhật size
	sizeInDesk--;
}

// HÀM NÀY DÙNG ĐỂ THÊM KHÁCH HÀNG VÀO BÀN ĂN
// B1: Nếu trong hàng đợi có khách và trong bàn ăn còn dư chỗ ngồi -> thêm vào
// B1.1: Kiểm tra với biến thời gian để lấy ra khách hàng tới sớm nhất
// B1.2: Xóa nó trong hàng chờ 
// B1.2.1: Nếu trong hàng chờ chỉ có 1 người thì cập nhật queue = null
// B1.2.1: Nếu trong hàng chờ có >= 2 người 
// B2: Thêm khách vào bàn ăn -> giống RED nhưng bỏ qua các lệnh check return
void imp_res::insertCustomerQueueToInDesk(){
	// B1: Nếu trong hàng đợi có khách và trong bàn ăn còn dư chỗ ngồi -> thêm vào
	while(sizeInQueue != 0 && sizeInDesk != MAXSIZE){
		// trỏ tới hàng chờ -> thằng tới sớm nhất trong hàng chờ
		customer* CustomerQueueToDesk = Queue;

		// trỏ tới biến thời gian
		TIME* temp = CusTimeHead;
		// B1.1: Kiểm tra với biến thời gian để lấy ra khách hàng tới sớm nhất
		while(temp != nullptr){
			// nếu tìm ra thì thêm vào
			if(temp->data == CustomerQueueToDesk){
				temp->Desk_true_Queue_false = true;
				break;
			}
			temp = temp->next;
		}
		// B1.2: Xóa nó trong hàng chờ 
		// B1.2.1: Nếu trong hàng chờ chỉ có 1 người thì cập nhật queue = null
		if(sizeInQueue == 1) Queue = nullptr;
		// B1.2.1: Nếu trong hàng chờ có >= 2 người 
		else{
			Queue->next->prev = Queue->prev;
			Queue->prev->next = Queue->next;
			// cập nhật lại customerQueue
			Queue = Queue->next;
		}
		// cập nhật size
		sizeInQueue--;

		
		// B2: Thêm khách vào bàn ăn -> giống RED nhưng bỏ qua các lệnh check return
		if(sizeInDesk == 0){
			Desk = CustomerQueueToDesk;
			CustomerQueueToDesk->next = CustomerQueueToDesk->prev = CustomerQueueToDesk;
		}
		else{
			if(sizeInDesk >= MAXSIZE / 2){
				customer* temp = Desk;
				for(int i = 0; i < sizeInDesk; i++){
					if(abs(CustomerQueueToDesk->energy - temp->energy) > abs(CustomerQueueToDesk->energy - Desk->energy)) Desk = temp;		
					temp = temp->next; 
				}
			
			}
			if(CustomerQueueToDesk->energy >= Desk->energy){
				CustomerQueueToDesk->next = Desk->next;
				CustomerQueueToDesk->prev = Desk;
				Desk->next = CustomerQueueToDesk->next->prev = CustomerQueueToDesk;
			}
			else{
				CustomerQueueToDesk->next = Desk;
				CustomerQueueToDesk->prev = Desk->prev;
				Desk->prev = CustomerQueueToDesk->prev->next = CustomerQueueToDesk;
			}
		}	
		Desk = CustomerQueueToDesk;
		sizeInDesk++;
	}
}
void imp_res::deleteCustomerByTime(TIME* deleteCus){
	// B2.1: Trường hợp thằng đó là người duy nhất trong bàn ăn và hàng chờ thì cập nhật null cho các biến thời gian 
	if(sizeInQueue + sizeInDesk == 1) CusTimeHead = CusTimeTail = nullptr;
	// B2.2: Trường hợp thằng cần xóa là vào sớm nhất
	else if(deleteCus == CusTimeHead){
		deleteCus->next->prev = nullptr;
		CusTimeHead = CusTimeHead->next;
	}
	// B2.3: Trường hợp thằng cần xóa là vào gần nhất
	else if(deleteCus == CusTimeTail){
		deleteCus->prev->next = nullptr;
		CusTimeTail = CusTimeTail->prev;
	}
	// B2.4: Trường hợp thằng cần xóa là ở chính giữa danh sách
	else{
		deleteCus->next->prev = deleteCus->prev;
		deleteCus->prev->next = deleteCus->next;
	}
}

// B1: Trường hợp không có ai -> return
// B2: Đuổi theo biến thời gian: sớm nhất -> gần nhất
// B2.1: Đuổi theo thời gian
// B2.2: Sau khi đã duyệt và cập nhật thì bắt đầu delete cái thằng đó ra 
void imp_res::BLUE(int num){
	// B1: Trường hợp không có ai -> return
	if(sizeInDesk == 0) return;

	// B2: Đuổi theo biến thời gian: sớm nhất -> gần nhất
	for(int i = 0; i < num && sizeInDesk != 0; i++){
		// Biến cần xóa là người vào sớm nhất
		TIME* customerTimeDelete = CusTimeHead;
		// Duyệt tìm cái thằng đó 
		while(customerTimeDelete != nullptr){
			// nếu thằng đó đang ở trong bàn ăn thì xóa nó
			if(customerTimeDelete->Desk_true_Queue_false) break;
			customerTimeDelete = customerTimeDelete->next;
		}
		// B2.1: Đuổi theo thời gian
		deleteCustomerByTime(customerTimeDelete);
		// B2.2: Sau khi đã duyệt và cập nhật thì bắt đầu delete cái thằng đó ra 
		this->deleteCustomerInDesk(customerTimeDelete);
		delete customerTimeDelete;
	}

//* Bước 2 xử lý đưa khách hàng từ hàng chờ vào bàn ăn
	//& đuổi xong rồi đưa mấy em từ hàng chờ vào ăn kiếm siền thôi
	this->insertCustomerQueueToInDesk();
}

// HÀM NÀY DÙNG ĐỂ XÓA KHÁCH HÀNG TRONG HÀNG CHỜ
// B1: Nếu trong hàng chờ chỉ có 1 người -> null
// B2: Nếu có >= 2 người trong hàng chờ
void  imp_res::deleteCustomerInQueue(TIME* customerTimeDelete){
	// B1: Nếu trong hàng chờ chỉ có 1 người -> null
	if(sizeInQueue == 1) Queue = nullptr;

	// B2: Nếu có >= 2 người trong hàng chờ
	else{
		// tạo biến customer trỏ tới data của CustomerTime
		customer* customerDelete = customerTimeDelete->data;
		// cập nhật next và prev
		customerDelete->next->prev = customerDelete->prev;
		customerDelete->prev->next = customerDelete->next;

		// Cập nhật lại Queue
		if(customerDelete == Queue) Queue = Queue->next;	
	}

	// Cập nhật lại size
	sizeInQueue--;
}


// B1: không có khách để đuổi
// B2: Có khách -> tính tổng năng lượng của thuật sư và oán linh
// B3: Duyệt từ khách hàng vào gần nhất in ra và xóa
// B3.1: Kiểm tra điều khiện nếu (oán linh < chú thuật sư && energy > 0) -> delete oán linh
//                         nếu (oán linh > chú thuật sư && energy < 0) -> delete chú thuật sư    
// B3.1.1: Xóa theo thời gian
// B3.1.2: Xóa thằng đó và cập nhật lại
// B3.2: Cập nhật temp
// B4: Thêm khách từ hàng đợi vào bàn ăn
void imp_res::DOMAIN_EXPANSION(){
    // B1: không có khách để đuổi
	if(sizeInDesk == 0) return;

	// B2: Có khách -> tính tổng năng lượng của thuật sư và oán linh
	int totalWizard = 0; // biến dùng để lưu tổng năng lượng của thuật sư
	int totalSpirit = 0; // biến dùng để lưu tổng năng lượng của oán linh

	TIME* temp = CusTimeHead;
	while(temp != nullptr){
	 	if(temp->data->energy > 0) totalWizard += temp->data->energy;
		else totalSpirit += temp->data->energy;
		temp = temp->next;
	}

	// B3: Duyệt từ khách hàng vào gần nhất in ra và xóa
	temp = CusTimeTail;
	while(temp != nullptr){
		// B3.1: Kiểm tra điều khiện nếu (oán linh < chú thuật sư && energy > 0) -> delete oán linh
		//                           nếu (oán linh > chú thuật sư && energy < 0) -> delete chú thuật sư      
		if(abs(totalSpirit) > totalWizard && temp->data->energy > 0 || abs(totalSpirit) <= totalWizard && temp->data->energy < 0 )
			temp->data->print();
		temp = temp->prev;
	}

	temp = CusTimeHead;	
	while(temp != nullptr){
		if(abs(totalSpirit) > totalWizard && temp->data->energy > 0 || abs(totalSpirit) <= totalWizard && temp->data->energy < 0 ){
			TIME* customerTimeDelete = temp; 
			temp = temp->next; // di chuyển nó lên tiếp theo để xóa

			// B3.1.1: Xóa theo thời gian
			deleteCustomerByTime(customerTimeDelete);

			// B3.1.2: Xóa thằng đó và cập nhật lại
			if(customerTimeDelete->Desk_true_Queue_false) this->deleteCustomerInDesk(customerTimeDelete);
			else this->deleteCustomerInQueue(customerTimeDelete);
			delete customerTimeDelete;
		}
		// B3.2: Cập nhật temp
		else temp = temp->next;
	}

    // B4: Thêm khách từ hàng đợi vào bàn ăn
	this->insertCustomerQueueToInDesk();
}

// HÀM NÀY CŨNG ĐỂ SWAP 2 NODE (SWAP ĐỊA CHỈ)
// B1: temp = head
// B2: head = tail
// B3: tail = temp
void imp_res::swapAddress(customer* head, customer * tail){
	customer* temp = new customer();
	// B1: temp = head
	temp->next = head->next;
	temp->prev = head->prev;
	head->next->prev = head->prev->next = temp;

	// B2: head = tail
	head->next = tail->next;
	head->prev = tail->prev;
	tail->next->prev = tail->prev->next = head;

	// B3: tail = temp
	tail->next = temp->next;
	tail->prev = temp->prev;
	temp->next->prev = temp->prev->next = tail;
	delete temp; 
}	

// tìm khách hàng thuật sư head => prev
Restaurant::customer* imp_res::WizardHead(customer* x){
	for(int i = 0; i < sizeInDesk; i++){
		if(x->energy > 0) return x;
		x = x->prev;
	}
	return x;
}

// tìm khách hàng thuật sư tail => next
Restaurant::customer* imp_res::WizardTail(customer* x){
	for(int i = 0; i < sizeInDesk; i++){
		if(x->energy > 0) return x;
		x = x->next;
	}
	return x;
}
// B1: Tìm ra head của thuật sư
// B2: Tìm ra tail của thuật sư
// B3: Nếu energy < 0 thì return vì k phải thuật sư
// B4: Swap head và tail
void imp_res::Wizard(customer* x){
	// B1: Tìm ra head của thuật sư
	customer* head = this->WizardHead(x); 
	// B2: Tìm ra tail của thuật sư
	customer* tail = this->WizardTail(x->next); 

	// B3: Nếu energy < 0 thì return vì k phải thuật sư
	if(head->energy < 0) return;
	// B4: Swap head và tail
	while(head != tail){
		this->swapAddress(head, tail);
		swap(head, tail);
		// Tiếp tục lặp lại B1 B2
		head = this->WizardHead(head->prev);
		// Nếu chỉ còn 1 khách chưa swap thì return
		if(head == tail) return;		
		tail = this->WizardTail(tail->next); 
	}
}

// tìm khách hàng oán linh head => prev
Restaurant::customer* imp_res::SpiritHead(customer* x){
	for(int i = 0; i < sizeInDesk; i++){
		if(x->energy < 0) return x;
		x = x->prev;
	}
	return x;
}

// tìm khách hàng oán linh tail => next
Restaurant::customer* imp_res::SpiritTail(customer* x){
	for(int i = 0; i < sizeInDesk; i++){
		if(x->energy < 0) return x;
		x = x->next;
	}
	return x;
}
// B1: Tìm ra head của oán linh
// B2: Tìm ra tail của oán linh
// B3: Nếu energy > 0 thì return vì k phải oán linh
// B4: Swap head và tail
void imp_res::Spirit(customer* x){
	// B1: Tìm ra head của oán linh
	customer* head = this->SpiritHead(x); 
	// B2: Tìm ra tail của oán linh
	customer* tail = this->SpiritTail(x->next); 

	// B3: Nếu energy > 0 thì return vì k phải oán linh
	if(head->energy > 0) return;
	// B4: Swap head và tail
	while(head != tail){
		this->swapAddress(head, tail);
		swap(head, tail);
		// Tiếp tục lặp lại B1 B2
		head = this->SpiritHead(head->prev);
		// Nếu chỉ còn 1 khách chưa swap thì return
		if(head == tail) return;		
		tail = this->SpiritTail(tail->next); 
	}
}

// B1: Nếu trong bàn ăn có khách >= 2 -> tiến hành đảo
// B1.1: Đảo oán linh
// B1.2: Đảo thuật sư
void imp_res::REVERSAL(){
	// B1: Nếu trong bàn ăn có khách >= 2 -> tiến hành đảo
	if(sizeInDesk >= 2){
		// B1.1: Đảo oán linh trước thuật sư sau
		if(Desk->energy > 0){	
			Spirit(Desk);
			Wizard(Desk);
		}
		// B1.2: Đảo thuật sư trước oán linh sau
		else{
			Wizard(Desk);
			Spirit(Desk);
		}
	}
}

// B1: Nếu số lượng trong bàn ăn < 4 thì return
// B2: Cho biến i j chạy lần lượt trong bàn ăn để check
// 	   Điều kiện 1: tổng energy phải nhỏ hơn lần trước đó và số lượng phải >= 4
// 	   Điều kiện 2: nếu tổng energy bằng lần trước thì phải có chiều dài lớn hơn lần trước và số lượng phải >= 4
// B3: Tìm phần tử nhỏ nhất trong cách phần tử vừa tìm được và in ra theo chiều kim đồng hồ (->next)
//     Nếu tìm thấy phần tử đó nằm ở giữa head và tail thì in từ phần tử đó -> tail và in từ head -> min vì head = tail->next ...
void imp_res::UNLIMITED_VOID(){
	// B1: Nếu số lượng trong bàn ăn < 4 thì return
	if(sizeInDesk <= 3) return;

	// B2: Cho biến i j chạy lần lượt trong bàn ăn để check
	else{
		int minTotal = 2147483647; //result
		int lenMax = 0;	// chiều dài chuỗi
		customer* head = nullptr; // node đầu 
		customer* tail = nullptr; // node cuối
		customer* ti = Desk; //! khách hàng i
		for(int i = 0; i < sizeInDesk; i++){
			int sumEnergy = 0;  //! tổng từ chạy i -> i + sizeCustomerInDesk
			customer* tj = ti; //! khách hàng j
			for(int j = 0; j < sizeInDesk; j++){
				// tính tổng TOTAL, cập nhật head và tail khi số khách > 4 và TOTAL nhỏ bằng
				sumEnergy +=  tj->energy;
				if(j >= 3 && (minTotal > sumEnergy || minTotal == sumEnergy && j + 1 >= lenMax)){
					head = ti;
					tail = tj;
					lenMax = j + 1;
					minTotal = sumEnergy;
				}

				tj = tj->next;
			}
			ti = ti->next;
		}			
		
		// B3: Tìm phần tử nhỏ nhất trong cách phần tử vừa tìm được và in ra theo chiều kim đồng hồ (->next)
		customer* min = head;
		customer* temp = head->next; // bắt đầu duyệt từ head->next
		int sizeHeadToMin = 1; // size để bắt đầu duyệt chuỗi head to min
		int sizeHeadToTail = 1; // size để bắt đầu duyệt min to tail = head to tail - head to min
		// tìm min
		while(temp != tail->next){
			sizeHeadToTail ++;
			if(min->energy > temp->energy){
				min = temp;
				sizeHeadToMin = sizeHeadToTail;
			}
			temp = temp->next;
		}
		// print từ min -> tail 
		for(int i = 0; i <= sizeHeadToTail - sizeHeadToMin; i++){
			min->print();
			min = min->next;
		}
		
		// print từ head -> min
		for(int i = 0; i < sizeHeadToMin - 1; i++){
			head->print();
			head = head->next;
		}
	}
	
}


// HÀM NÀY DÙNG ĐỂ TÌM VỊ TRÍ CỦA CUSTOMER
// Duyệt for để tìm và trả về
imp_res::customer* imp_res::getCustomer(customer* x, int idx){
	for(int i = 0; i < idx; i++) x = x->next;
	return x;
}

// HÀM NÀY DÙNG ĐỂ CHECK AI ĐẾN TRƯỚC 
// Giả sử swap 4(1) và 4(2) thì:
// temp = head = 4(1)
// temp->data = 4(1) -> return false (không swap)
// ngược lại
// còn lại thì true
bool imp_res::checkTime(customer* a, customer* b){
	TIME* temp = CusTimeHead;
	while(temp != NULL){
		if(temp->data == a) return false;
		else if (temp->data == b) return true;
		temp = temp->next;
	}
	return true;
}
 

// HÀM NÀY DÙNG ĐỂ TRẢ VỀ SỐ LƯỢNG CẦN XÓA VÀ SORT THEO SHELL SORT
int imp_res::sort(customer* head, int n, int increment) {
	int count = 0; // số lượng swap để lát xóa
	// B1: Tạo vòng lặp để so sánh Q[j] Q[j+1] với kích thước của mảnh hiện tại là lenSort - j
	for(int i = increment; i < n; i += increment){
		// B2: Bắt đầu sort trong mảnh
		for(int j = i; j >= increment; j -= increment){
			
			customer* s1 = this->getCustomer(head, j);
			customer* s2 = this->getCustomer(head, j - increment);

			//điệu kiện dừng
			if(abs(s1->energy) < abs(s2->energy) || (abs(s1->energy) == abs(s2->energy) && checkTime(s1,s2))) break;

			// đổi chỗ energy cao lên, xuống dưới 
			swapAddress(s1, s2);
			
			// Trường hợp swap là Queue thì cập nhật lại 
			// TH1: Queue = s2
			// Th2: head = s2
			// Thì phải đổi ngược lại cho s1
			if(Queue == s2) Queue = s1;
			//& đưa thằng mạnh lên lại trúng chỗ thằng head đầu dãy thôi để cập nhật lại
			if(head == s2) head = s1;

			count++;
		}
	}
	return count;
}

// 1. Ban đầu, giải thuật bắt đầu với một giá trị ban đầu của increment là n/2, tức là chia mảng thành
// 	  hai phần bằng nhau và sắp xếp từng phần
// 2. Giải thuật tiếp tục thực hiện vòng lặp cho đến khi increment giảm xuống dưới 2 (cụ thể là 1)
// 3. Trong mỗi vòng lặp, mảng được chia thành các phần tử con (sublist) bắt đầu từ các vị trí j từ 0
//    đến i-1 (trong đó i là giá trị của increment tại thời điểm đó)
// 4. Đối với mỗi phần tử con (sublist) này, hàm inssort2 được gọi để sắp xếp các phần tử trong sublist
//    đó bằng thuật toán sắp xếp chèn biến thể (insertion sort variant) với increment là i.
// 5. Sau khi hoàn thành vòng lặp con này, increment giảm đi một nửa và lại tiếp tục với một vòng lặp
//    khác cho đến khi increment giảm xuống dưới 2
// 6. Cuối cùng, khi increment bằng 1, mảng sẽ được sắp xếp một lần nữa bằng thuật toán inssort2 với
//    increment bằng 1, hoàn thành quá trình sắp xếp.

// B1: Có 1 người thì k sort -> return 
// B2: Tìm max energy trong hàng chờ
//     Vì đây là biến thời gian nên for (0 -> sizeDesk + sizeQueue)
// B2.1: Tìm kích thước danh sách cần sort
// B2.2: Bắt đầu sort
// B2.3: Xóa 
void imp_res::PURPLE(){
	// B1: Có 1 người thì k sort -> return 
	if(this->sizeInQueue <= 1) return;
	else{
		int lenDelele = 0;
		// B2: Tìm max energy trong hàng chờ
		TIME* temp = CusTimeTail;
		TIME* max = nullptr;
		while(temp != nullptr){
			if(temp->Desk_true_Queue_false == 0 && max == nullptr) max = temp;
			else if(temp->Desk_true_Queue_false == 0 && abs(temp->data->energy) > abs(max->data->energy)) 
				max = temp;
			temp = temp->prev;
		}
		
		// B2.1: Tìm kích thước danh sách cần sort
		int lenSort = 1; 
		customer* s = Queue;
		while(s != max->data){
			lenSort++;
			s = s->next;
		}
		
		// B2.2: Bắt đầu sort
		for(int i = lenSort / 2; i > 2; i /= 2){
			// phần tử bắt đầu của sort từ 0 -> i 	
			for(int j = 0; j < i; j++){
				// sort với index bắt đầu Queue[j]
				// mỗi bước nhảy là i -> nghĩa là so sánh Q[j], Q[j+i], ...
				// lenSort - j là kích thước của mảng hiện tại
				lenDelele += sort(this->getCustomer(Queue, j), lenSort - j, i);
			}
		}
		lenDelele += sort(Queue, lenSort, 1);
		
		// B2.3: Xóa 
		this->BLUE(lenDelele % MAXSIZE);
	}
}

void imp_res::LIGHT(int num){
	// hàng chờ Queue
	if(num == 0){
		customer* temp = Queue;
		for(int i = 0; i < sizeInQueue; i++){
			temp->print();
			temp = temp->next;
		}
	}
	// Desk chiều kim đồng hồ
	else if(num > 0){	
		customer* temp = Desk;
		for(int i = 0; i < sizeInDesk; i++){
			temp->print();
			temp = temp->next;
		}
	}
	// Desk ngược chiều kim đồng hồ
	else 
	{
		customer* temp = Desk;
		for(int i = 0; i < sizeInDesk; i++){
			temp->print();
			temp = temp->prev;
		}
	}
}