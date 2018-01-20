DROP PROCEDURE if exists enroll;

delimiter $$

CREATE PROCEDURE enroll 
(
    in pusername int, 
    in pcourseid varchar(10), 
    in psemester varchar(2), 
    in pyear int, 
    out red varchar(100), 
    out req varchar(100), 
    out req2 varchar(100)
)

Begin

declare prereq varchar(10);
declare done int;
declare flag int default 0;
declare cur1 cursor for select prerequoscode from requires where uoscode=pcourseid;
declare continue handler for not found set done = 1;

DECLARE exit handler for sqlexception
  BEGIN
    -- ERROR
  ROLLBACK;
END;

DECLARE exit handler for sqlwarning
 BEGIN
    -- WARNING
 ROLLBACK;
END;

if exists (select prerequoscode from requires where uoscode=pcourseid)
then

open cur1;
dbloop1:loop
fetch cur1 into prereq; 
if done=1
then 
leave dbloop1;
end if;

if not exists (select * from transcript where studid=pusername and uoscode=pcourseid)
then
	if exists (select * from transcript where studid=pusername and prereq=uoscode and grade in ('P','CR'))
    then
		if exists (select * from uosoffering where uoscode=pcourseid and maxenrollment>enrollment and year=pyear and semester=psemester)
        then
			set flag=10;
		else
			set flag=1;
        end if;
	else
		set flag=3;
        if (isnull(req))
		then 
		set req=prereq;
		elseif prereq<>req 
		then 
		set req2=prereq;
		end if;
    end if;
else
	set flag=2;
end if;
end loop dbloop1;
close cur1;

else
	if not exists (select * from transcript where studid=pusername and uoscode=pcourseid)
	then
		if exists (select * from uosoffering where uoscode=pcourseid and maxenrollment>enrollment and year=pyear and semester=psemester)
		then
			set flag=10;
		else
			set flag=1;
		end if;
	else
		set flag=2;
	end if;
end if;


if not exists (Select * from uosoffering where uoscode=pcourseid)
then set flag=0;
end if;

if (not(isnull(req)) or not(isnull(req2)))
then set flag=3;
end if;

if flag=10 then
START TRANSACTION;
	insert into transcript (studid,uoscode,semester,year) values (pusername, pcourseid, psemester, pyear);
	update uosoffering set enrollment=enrollment+1 where uoscode=pcourseid and year=pyear and semester=psemester;
	set red='___REGISTRATION SUCCESSFULL___';
COMMIT;
elseif flag=1 then
	set red='___SORRY! MAXIMUM CAPACITY OF CLASS HAS BEEN REACHED___';
elseif flag=2 then
	set red='___UNABLE TO REGISTER. YOU HAVE TAKEN THIS COURSE PREVIOUSLY___';
elseif flag=3 then
	set red='___PREREQUISITES NOT MET. THE FOLLOWING ARE THE PREREQUISITES: ___';
elseif flag=0 then
	set red='___INVALID COURSE NUMBER___';
end if;

select red,req,req2;
end

$$
delimiter ;

