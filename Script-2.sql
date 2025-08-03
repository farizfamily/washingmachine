select top 10 
*
from smOrder order by PODate desc

SELECT * FROM COMPANY WHERE CompanyID = 18532


select top 31 * from users where companyid='18532'
select * from users where UserID = 6002;

SELECT c.TABLE_NAME, COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS c 
join INFORMATION_SCHEMA.TABLES t on c.TABLE_NAME = t.TABLE_NAME 
WHERE
t.TABLE_TYPE ='BASE TABLE' AND 
column_name LIKE '%email%'


SELECT TOP 10 * FROM Logins l WHERE LOGIN='review@hexa.com'

select u.*,'batas' as x,l.* from Users u join Logins l on u.UserID = l.UserId 
where 
u.CompanyID = 585
l.[Login] ='review@hexa.com';

select companyId,count(1) n from smOrderMoviantoDocument a  join Users b on a.CreatedBy = b.UserID 
group by b.CompanyID 
order by n desc; -- 585	28448



select 
sd.CompanyID ,
count(1) n
from
smOrder so join
smDocument sd on so.SMDocID =sd.SMDocID 
group by sd.CompanyID order by n desc;

select top 100 so.*,'xxxx' batas, sd.*
from
smOrder so join
smDocument sd on so.SMDocID =sd.SMDocID 
where sd.CompanyID = 585 order by sd.CreatedOn desc;

select top 5 so.DeliveryToCompany , sd.DocDate , sd.Total , year(sd.DocDate) as tahun
from
smOrder so join
smDocument sd on so.SMDocID =sd.SMDocID 
where sd.CompanyID = 585 order by sd.CreatedOn desc;



select  year(sd.DocDate) as tahun, sum(sd.Total )*21000
from
smOrder so join
smDocument sd on so.SMDocID =sd.SMDocID 
where sd.CompanyID = 585 
group by year(sd.DocDate )
order by year(sd.DocDate) desc;
order by sd.CreatedOn desc;


select  * from smOrder where PODate BETWEEN '2025-05-22' and '2025-07-24' order by PODate desc

select  * from INFORMATION_SCHEMA.TABLES t where t.TABLE_TYPE ='BASE TABLE' AND  t.TABLE_SCHEMA ='dbo' AND TABLE_NAME LIKE '%stock%' ORDER BY t.TABLE_NAME


select * from emDescriptionType edt where companyID=585


select  * from accAccountChart where CompanyID =585

select * from FinancialYear fy where fy.CompanyID =585 order by fy.YearStart desc

select top 10  * from smStockIn msi ;--where msi.CompanyId =585

select top 10 * from smOrderMoviantoDocument ;

SELECT top 10 * from emExpense ee ;

SELECT top 10 * from emClaim ec ;

select companyId,count(1) n from emExpense ee join emClaim ec on ee.ClaimID =ec.ClaimID 
group by ec.CompanyID order by n desc;

select top 5 ee.*,'XXX' batas, ec.* from emExpense ee join emClaim ec on ee.ClaimID =ec.ClaimID 
where ec.CompanyID =585 ORDER by ec.OnDate DESC 

select top 10 ec.claimId,sum(ee.amount),ec.amount from emExpense ee join emClaim ec on ee.ClaimID =ec.ClaimID 
where ec.CompanyID =18532 group by ec.ClaimID, ec.Amount order by ec.ClaimID  DESC ;

-- TAC 18532
-- Rivo 585
