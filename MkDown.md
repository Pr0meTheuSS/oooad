# Система бронирования аудиторий

Версия: учебная документация для Java CLI-приложения.

---

## Vision

Консольное Java-приложение для бронирования аудиторий в учебном заведении. Цель — продемонстрировать студентам ценность требований, анализа и дизайна. Поддержка ролей (STUDENT, ADMIN), работа с комнатами, создание/отмена броней, валидация времени, хранение in-memory.

---

## 1. Требования

### Функциональные

- FR1 — Аутентификация: вход по уникальному логину.
- FR2 — Список аудиторий: минимум 3 комнаты. (можно захардкодить)
- FR3 — Просмотр слотов по комнате/дате.
- FR4 — Создание брони.
- FR5 — Просмотр своих броней.
- FR6 — Отмена своей брони.
- FR7 — Admin: просмотр всех броней (только с ролью ADMIN).
- FR8 — Admin: отмена любой брони (только с ролью ADMIN).
- FR9 — Запрет брони в прошлом; end > start.

### Нефункциональные

- NFR1 — In-memory хранение.
- NFR2 — Интерфейс: CLI.
- NFR3 — Валидация и обработка ошибок.
- NFR4 — Репозитории через интерфейсы (подмена на БД возможна).

---

## 2. Acceptance Tests (AT)

- AT-1: `login <user>` → успешный логин или ошибка, если пользователь не существует.
- AT-2: `list-rooms` → вывод комнат.
- AT-3: `show-slots <roomId> <date>` → показывает свободные слоты.
- AT-4: `book <roomId> <start> <end>` → создаёт бронь, если валидно.
- AT-5: `my-bookings` → показывает брони пользователя.
- AT-6: `cancel <bookingId>` → отмена своей брони.
- AT-7: `all-bookings` (admin) → показывает все брони, только если роль ADMIN.
- AT-8: `admin-cancel <bookingId>` → отмена любой брони, только если роль ADMIN.

---

## 3. User Stories с CLI-примерами и диаграммами

### US-1: Login **Acceptance**: login alice → Logged in, иначе Unknown user. **CLI**:
```bash
> login alice
✅ Logged in as alice

> login unknown
❌ Unknown user
```

![alt text](image.png)

**Sequence**:
```plantuml
@startuml
actor User
participant "Console UI" as UI
participant "AuthService" as Auth
participant "UserRepo" as Repo
User -> UI: enter login
UI -> Auth: authenticate(login)
Auth -> Repo: findUser(login)
alt user exists
    Repo --> Auth: User
    Auth --> UI: success
    UI -> User: show "Logged in"
else not found
    Repo --> Auth: null
    Auth --> UI: error
    UI -> User: show "Login failed"
end
@enduml
```

![alt text](image-1.png)
**Activity**:
```plantuml
@startuml
  start
  :Read login;
  if (empty?) then (yes)
    :Show error;
    stop
  else (no)
    :findUser(login);
    if (exists?) then (yes)
      :Create session;
      :Show success;
    else (no)
      :Show error;
    endif
  endif
  stop
@enduml
```

--- 

### US-2: View Rooms **Acceptance**: list-rooms → ≥ 3 комнаты. 
**CLI**:
```bash
> list-rooms
ROOMS:
  [1] Room A (capacity 20)
  [2] Room B (capacity 50)
  [3] Room C (capacity 10)
```
![alt text](image-2.png)
**Sequence**:
```plantuml
@startuml
  actor User
  participant UI
  participant RoomService as RS
  participant RoomRepo as RR

    User -> UI: list-rooms
    UI -> RS: getRooms()
    RS -> RR: fetchAll()
    RR --> RS: rooms
    RS --> UI: rooms
    UI -> User: display list
@enduml
```

![alt text](image-3.png)
**Activity**:
```plantuml
@startuml
  start
    :Select list-rooms;
    :Fetch rooms;
    if (empty) then (yes)
      :Show "No rooms";
    else (no)
      :Display rooms;
    endif
  stop
@enduml
```

--- 
### US-3: View Slots 
**Acceptance**: show-slots 1 2025-10-07 → список свободных слотов. 

**CLI**:
```bash
> show-slots 1 2025-10-07
Room A schedule:
09:00-11:00 free
11:00-13:00 booked
13:00-15:00 free
```

**Sequence**:
```plantuml
@startuml
actor User
participant UI
participant BookingService as BS
participant BookingRepo as BR

User -> UI: show-slots(room,date)
UI -> BS: getAvailable(room,date)
BS -> BR: fetchBookings(room,date)
BR --> BS: list<Bookings>
BS -> BS: calculateFreeSlots(list)
BS --> UI: list<FreeSlots>
UI -> User: display
@enduml
```

![alt text](image-4.png)

**Activity**:
```plantuml
@startuml
start
:User inputs roomId, date;
:UI sends request to BookingService;
:BookingService fetches bookings from repository;
:Sort bookings by start time;

if (bookings empty?) then (yes)
  :Create full-day slot (09:00–22:00);
  :Return [single slot];
else (no)
  :Set startOfDay = 09:00;
  :Set endOfDay = 22:00;
  :cursor = startOfDay;

  repeat
    :Take next booking;
    if (cursor < booking.start) then (yes)
      :Add slot [cursor, booking.start];
    endif
    :cursor = booking.end;
  repeat while (more bookings?)

  if (cursor < endOfDay) then (yes)
    :Add slot [cursor, endOfDay];
  endif
endif

:Return list of free slots to UI;
:UI displays slots to user;
stop
@enduml
```
---

### US-4: Create Booking

**Acceptance**: `book 1 2025-10-07T09:00 2025-10-07T11:00` → создаётся, если нет конфликта.

**CLI**:

```bash
> book 1 2025-10-07T09:00 2025-10-07T11:00
✅ Booking created [id=42]

> book 1 2025-01-01T09:00 2025-01-01T11:00
❌ Cannot book in the past
```

**Sequence**:

![alt text](image-5.png)

```plantuml
@startuml
  actor User
  participant UI
  participant BookingService as BS
  participant RoomRepo as RR
  participant BookingRepo as BR

    User -> UI: book(roomId, start, end)
    UI -> BS: createBooking(user, roomId, start, end)
    BS -> RR: findRoom(roomId)
    RR --> BS: Room
    BS -> BR: fetchBookings(roomId, date)
    BR --> BS: bookings
    BS -> BS: validate time & conflicts
    alt valid
      BS -> BR: save(Booking)
      BR --> BS: Booking(id)
      BS --> UI: success
    else invalid
      BS --> UI: error
    end
    UI -> User: show result

@enduml
```

**Activity**:

![alt text](image-6.png)

```plantuml
@startuml
  start
    :Read roomId, start, end;
    :Validate date (start > now(), end>start);
    :Check conflicts;
    if (valid?) then (yes)
      :Create Booking;
      :Save to repo;
      :Show success;
    else (no)
      :Show error;
    endif
  stop
@enduml
```

---

### US-5: My Bookings

**CLI**:

```bash
> my-bookings
Your bookings:
[42] Room A 2025-10-07 09:00-11:00 ACTIVE
```

**Sequence**:

```plantuml
@startuml
  actor User
  participant UI
  participant BookingService as BS
  participant BookingRepo as BR

    User -> UI: my-bookings
    UI -> BS: listBookingsByUser(login)
    BS -> BR: fetchByOwner(login)
    BR --> BS: list
    BS --> UI: list
    UI -> User: display
@enduml
```

**Activity**:
![alt text](image-7.png)

```plantuml
@startuml
  start
    :Command my-bookings;
    :Fetch user's bookings;
    :Display list with status;
  stop
@enduml
```

---

### US-6: Cancel Own Booking

**CLI**:

```bash
> cancel 42
✅ Booking 42 cancelled
```

**Sequence**:

![alt text](image-8.png)
```plantuml
@startuml
actor User
participant UI
participant AuthService as Auth
participant BookingService as BS
participant BookingRepo as BR

User -> UI: cancel(bookingId)
UI -> Auth: getCurrentUser()
Auth --> UI: User
UI -> BS: cancelBooking(currentUser, bookingId)

BS -> BR: findBookingById(bookingId)
alt booking not found
  BR --> BS: null
  BS --> UI: error("Booking not found")
else booking found
  BR --> BS: Booking
  BS -> BS: check ownership & status
  alt not owner
    BS --> UI: error("Access denied")
  else already cancelled
    BS --> UI: error("Already cancelled")
  else valid
    BS -> BR: updateStatus(CANCELLED)
    BR --> BS: ok
    BS --> UI: success("Booking cancelled")
  end
end
UI -> User: show result
@enduml
```

**Activity**:

```plantuml
@startuml
  start
    :Enter bookingId;
    :Find booking;
    :Check owner and status;
    if (valid?) then (yes)
      :Cancel booking;
      :Show success;
    else (no)
      :Show error;
    endif
  stop
@enduml
```

---

### US-7: Admin View All

**CLI**:

```bash
> all-bookings
ALL BOOKINGS:
[42] Room A by alice ACTIVE
[43] Room B by bob CANCELLED
```

**Sequence**:
![alt text](image-10.png)

```plantuml
@startuml
actor Admin
participant UI
participant AuthService as AS
participant BookingService as BS
participant BookingRepo as BR
Admin -> UI: all-bookings
UI -> AS: verifyRole(admin, ADMIN)
alt role invalid
  AS --> UI: Access Denied
  UI -> Admin: error (Access Denied)
else role ok
  AS --> UI: verified
  UI -> BS: getAllBookings()
  BS -> BR: fetchAll()
  BR --> BS: list
  BS --> UI: list
  UI -> Admin: display
end
@enduml
```

**Activity**:

```plantuml
@startuml
start
:Command all-bookings;
:Check if user.role == ADMIN;
if (admin?) then (yes)
  :Fetch all bookings;
  :Display list;
else (no)
  :Show access denied;
endif
stop
@enduml
```

---

### US-8: Admin Cancel Any

**CLI**:

```bash
> admin-cancel 42
✅ Booking 42 cancelled by admin
```

**Sequence**:

![alt text](image-9.png)

```plantuml
@startuml
actor Admin
participant UI
participant AuthService as Auth
participant BookingService as BS
participant BookingRepo as BR

Admin -> UI: admin-cancel(bookingId)
UI -> Auth: getCurrentUser()
Auth --> UI: AdminUser
UI -> BS: cancelBooking(AdminUser, bookingId)

BS -> BS: checkRole(ADMIN)
alt not admin
  BS --> UI: error("Access denied: admin only")
else ok
  BS -> BR: findBookingById(bookingId)
  alt booking not found
    BR --> BS: null
    BS --> UI: error("Booking not found")
  else booking found
    BR --> BS: Booking
    BS -> BS: checkStatus(Booking)
    alt already cancelled
      BS --> UI: error("Booking already cancelled")
    else active
      BS -> BR: updateStatus(CANCELLED, cancelledBy=AdminUser.login)
      BR --> BS: ok
      BS --> UI: success("Booking cancelled by admin")
    end
  end
end
UI -> Admin: show result
@enduml
```

**Activity**:

```plantuml
@startuml
start
:Input bookingId;
:Check if user.role == ADMIN;
if (admin?) then (yes)
  :Find booking;
  :Cancel booking;
  :Show confirmation;
else (no)
  :Show access denied;
endif
stop
@enduml
```

---

## 9. Диаграмма классов UI → Service → Repo → Domain

```plantuml
@startuml
package ui {
  class ConsoleUI {
    - authService : AuthService
    - roomService : RoomService
    - bookingService : BookingService
    + handleCommand(cmd: String, args: List<String>) : void
  }
}

package service {
  class AuthService {
    - userRepo : UserRepository
    + login(login: String) : Optional<User>
    + verifyRole(user: User, required: Role) : boolean
  }
  class RoomService {
    - roomRepo : RoomRepository
    + listRooms() : List<Room>
  }
  class BookingService {
    - bookingRepo : BookingRepository
    - roomRepo : RoomRepository
    + createBooking(user: User, roomId: int, start: LocalDateTime, end: LocalDateTime) : Optional<Booking>
    + cancelBooking(user: User, bookingId: int) : boolean
    + listBookingsByUser(login: String) : List<Booking>
    + getAllBookings(user: User) : List<Booking>
  }
}

package repository {
  interface UserRepository {
    + findUser(login: String) : Optional<User>
  }
  interface RoomRepository {
    + fetchAll() : List<Room>
    + findRoom(id: int) : Optional<Room>
  }
  interface BookingRepository {
    + fetchAll() : List<Booking>
    + fetchBookings(roomId: int, date: LocalDate) : List<Booking>
    + fetchByOwner(owner: String) : List<Booking>
    + findBookingById(id: int) : Optional<Booking>
    + save(booking: Booking) : Booking
    + updateStatus(id: int, status: BookingStatus) : void
  }
}

package domain {
  class User {
    - login : String
    - name : String
    - role : Role
  }
  class Room {
    - id : int
    - name : String
    - capacity : int
    - location : String
  }
  class Booking {
    - id : int
    - roomId : int
    - ownerLogin : String
    - startTime : LocalDateTime
    - endTime : LocalDateTime
    - status : BookingStatus
    - createdAt : LocalDateTime
    - cancelledBy : String
  }
  enum BookingStatus {
		ACTIVE,
		CANCELED
	}
}

ConsoleUI --> AuthService
ConsoleUI --> RoomService
ConsoleUI --> BookingService
AuthService --> UserRepository
RoomService --> RoomRepository
BookingService --> BookingRepository
BookingService --> RoomRepository
BookingRepository --> Booking
@enduml
```

---

## 10. Архитектурные заметки

Добавлены уточнения в сервисы и репозитории:

- Репозитории возвращают Optional для безопасной обработки отсутствующих данных.
- BookingService проверяет роль пользователя перед вызовом административных методов.
- BookingService выполняет валидацию времени и конфликтов.
- ConsoleUI агрегирует сервисы и маршрутизирует команды.
- Domain слой не зависит от инфраструктуры.

---

# Конец документа
