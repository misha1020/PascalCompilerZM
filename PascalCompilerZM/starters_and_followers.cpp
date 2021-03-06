#include "pch.h"

#include "starters_and_followers.h"

/* Множества КОДОВ символов, стартовых для различных обрабатываемых 	*/
/* конструкций:								*/



vector<int> codes_idstart{ ident,eolint };	
		/* стартовые символы некоторых 	*/
		/* обрабатываемых конструкций	*/
vector<int> codes_block{ labelsy,constsy,typesy,varsy,functionsy, proceduresy,beginsy,eolint };	
		/* стартовые символы разделов	*/
		/* описаний и раздела операторов*/
vector<int> codes_rightpar{ rightpar,eolint };
		/* правая скобка */
vector<int> codes_constant{ plus,minus,charc,stringc,ident,intc,floatc, eolint };	
		/* стартовые сcodes_rightparимволы конструкции constant	*/
vector<int> codes_typ{ packedsy,arrow,arraysy,filesy,setsy,recordsy, plus,minus,ident,leftpar,intc,charc,stringc,eolint };
		/* стартовые символы конструкции описания типа;	*/
		/* {plus,minus,ident,leftpar,intc,charc,stringc,eolint}-*/
		/* стартовые символы конструкции описания простого типа */
vector<int> codes_fixpart{ ident,semicolon,casesy,endsy,eolint };
		/* стартовые символы конструкции списка полей и кон-	*/
		/* струкции фиксированной части	записи			*/
vector<int> codes_casefield{ floatc,endsy,semicolon,plus,minus,charc, stringc,ident,intc,eolint };
		/* стартовые символы конструкции варианта       	*/
vector<int> codes_statement{ intc,endsy,elsesy,untilsy,ident,beginsy,ifsy,whilesy,repeatsy,forsy,casesy,withsy,semicolon,gotosy,eolint };
		/* стартовые символы конструкции <оператор>             */
		/* (при нейтрализации ошибок используются символы:      */
		/* {beginsy,ifsy,whilesy,repeatsy,forsy,casesy,withsy,  */
		/* semicolon,gotosy,eolint} )                           */
vector<int> codes_express{ plus, minus, leftpar, lbracket, notsy, ident, intc, floatc, charc, stringc, nilsy, eolint };
		/* стартовые символы выражения и простого выражения 	*/
vector<int> codes_termfact{ ident, leftpar, lbracket, notsy, intc, floatc, charc, stringc, nilsy, eolint };
		/* стартовые символы слагаемого и множителя 		*/


/* Множества кодов символов, ожидаемых после обработки различных кон-	*/
/* струкций:								*/

vector<int> acodes_block{ point,endoffile,eolint };	
		/* символы, следующие за конструкцией блока в основной 	*/
		/* программе			 			*/
vector<int> acodes_simpletype{ comma,rbracket,eolint };
		/* символы, ожидаемые сразу после вызова simpletype() во 	*/
		/* время анализа типа "массив" 				*/
vector<int> acodes_fixpart{ casesy,rightpar,endsy,eolint };	
		/* символы, ожидаемые сразу после анализа конструкции fixpart;	*/
		/* {rightpar,endsy,eolint} - символы, ожидаемые сразу 	*/
		/* после анализа списка полей при вызове reestrfields() */
		/* из casefield() ( а при вызове из complextype() ожи-	*/
		/* даемый символ только endsy )				*/
vector<int> acodes_typ{ endsy,rightpar,semicolon,eolint };	
		/* символы, ожидаемые сразу после анализа конструкции typ 	*/
		/* при вызове функции typ() из fixpart()		*/
vector<int> acodes_2constant{ comma,colon,eolint };	
		/* символы, ожидаемые 	*/
		/* сразу после анализа константы при вызове constant()	*/
		/* из casefield() и variant()				*/
vector<int> acodes_3const{ twopoints,comma,rbracket,eolint };	
		/* коды */
		/* символов, ожидаемых сразу после анализа константы 	*/
vector<int> acodes_listparam{ colon,semicolon,forwardsy,constsy,varsy,eolint }; 
		/* символы, ожидаемые сразу после списка параметров     */
		/* ( символы functionsy,proceduresy,beginsy уже есть в  */
		/*   followers)                                         */
vector<int> acodes_factparam{ comma,rightpar,eolint };
		/* символы, ожидаемые сразу после разбора фактических   */
		/* параметров процедур и функций                        */
vector<int> acodes_assign{ assign,eolint };
		/* символ, ожидаемый сразу после переменной в операторе */
		/* присваивания и в операторе for                       */
vector<int> acodes_compcase{ semicolon,endsy,eolint };
		/* символы, ожидаемые сразу после оператора в составном */
		/* операторе и после варианта в операторе варианта      */
vector<int> acodes_iftrue{ thensy,eolint };
		/* символ, ожидаемый сразу после условного выражения в  */
		/* операторе if						*/
vector<int> acodes_iffalse{ elsesy,eolint };
		/*символ, ожидаемый сразу после оператора ветви "истина"*/
		/* в операторе if					*/
vector<int> acodes_wiwifor{ comma,dosy,eolint };
		/* символы, ожидаемые сразу после переменной в заголовке*/
		/* оператора with;  { dosy,eolint} - символ, ожидаемый  */
		/* сразу после условного выражения в операторе while и  */
		/* сразу после выражения-второй границы изменения пара- */
		/* метра цикла в операторе for				*/
vector<int> acodes_repeat{ untilsy, semicolon, eolint };
		/* cимволs, ожидаемые сразу после оператора в теле      */
		/* оператора repeat					*/
vector<int> acodes_case1{ ofsy,eolint };
		/* символ, ожидаемый сразу после выбирающего выражения  */
		/* в операторе case					*/
vector<int> acodes_for1{ tosy,downtosy,eolint };
		/* символы, ожидаемые сразу после выражения-первой гра- */
		/* ницы изменения пераметра цикла в операторе for       */
vector<int> acodes_ident{ lbracket, arrow, point, eolint };
		/* ... после идентификатора в переменной 		*/
vector<int> acodes_index{ rbracket, comma, eolint };
		/* ... после индексного выражения при разборе массива	*/
vector<int> acodes_set1{ rbracket, twopoints, comma, eolint };
		/* ... после 1-го выражения в конструкторе множества 	*/


