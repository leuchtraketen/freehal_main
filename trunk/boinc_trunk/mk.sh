cd /home/boincadm/boinc_trunk/sched/
make && (

g++ -o upload upload.cpp

g++ -o set-limit set-limit.cpp
g++ -o get-limit get-limit.cpp

for x in 1 2 3 4 5
do
	cp validate_fast sample_trivial_validator sample_assimilator sample_work_generator delete_file request_file_list make_work get_file show_shmem send_file sched_driver wu_check db_dump census file_deleter feeder db_purge update_stats transitioner trickle_handler /home/boincadm/www/projects/freehal_at_home/bin/
	cp ../tools/{dir_hier_move,dir_hier_path,sign_executable,create_work} /home/boincadm/www/projects/freehal_at_home/bin/
	cp set-limit get-limit /home/boincadm/www/projects/freehal_at_home/bin/
	cp cgi fcgi upload /home/boincadm/www/projects/freehal_at_home/cgi-bin/
	cp -f validate_fast sample_trivial_validator sample_assimilator sample_work_generator delete_file request_file_list make_work get_file show_shmem send_file sched_driver wu_check db_dump census file_deleter feeder db_purge update_stats transitioner trickle_handler /home/boincadm/www/projects/freehal_at_home/bin/
	cp -f cgi fcgi upload /home/boincadm/www/projects/freehal_at_home/cgi-bin/
	chmod 0775 /home/boincadm/www/projects/freehal_at_home/cgi-bin/*
	chmod u+s /home/boincadm/www/projects/freehal_at_home/cgi-bin/*
done

)

