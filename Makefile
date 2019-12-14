client = client

server = server

FLAGS = gcc -g -Wall -Wextra -Werror

client_obj = client_obj/main.o client_obj/handlers.o\
			client_obj/read_input.o client_obj/login.o\
			client_obj/join.o client_obj/connect.o\
			client_obj/quit.o client_obj/msg.o\
			client_obj/receive_msg.o client_obj/show_msg.o\
			client_obj/leave.o client_obj/who.o\
			client_obj/msg_channel.o client_obj/send_recv.o\
     
server_obj = server_obj/main.o server_obj/handlers.o\
			server_obj/connect.o server_obj/select.o\
			server_obj/cmds.o server_obj/exec.o\
			server_obj/support.o server_obj/join.o\
			server_obj/leave.o server_obj/login.o\
			server_obj/remove_channel.o server_obj/msg.o\
			server_obj/send.o server_obj/who.o\
			server_obj/quit.o server_obj/msg_channel.o\
			server_obj/send_recv.o\

all : $(server) $(client)

$(server) : $(server_obj)
	make -C libft
	$(FLAGS) -o $(server) $(server_obj) libft/libft.a

$(client) : $(client_obj)
	make -C libft
	$(FLAGS) -o $(client) server_obj/select.o $(client_obj) libft/libft.a

$(client_obj): client_obj/%.o: client_src/%.c includes/client.h
	$(FLAGS) -c client_src/$*.c -o $@

$(server_obj): server_obj/%.o: server_src/%.c includes/server.h
	$(FLAGS) -c server_src/$*.c -o $@

clean:
	make clean -C libft/
	rm -f $(client_obj)
	rm -f $(server_obj)

fclean: clean
	make fclean -C libft/
	rm -f $(client)
	rm -rf $(server)

re: fclean all
