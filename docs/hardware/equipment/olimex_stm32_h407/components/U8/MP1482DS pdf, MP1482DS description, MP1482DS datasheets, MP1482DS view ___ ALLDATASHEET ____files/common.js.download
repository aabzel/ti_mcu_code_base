var strPath = window.location.hostname;
var strLang = "alldatasheet.com";

if(strPath=="alldatasheet.com" || strPath=="www.alldatasheet.com" || strPath=="distributor.alldatasheet.com" || strPath=="manufacture.alldatasheet.com" || strPath=="html.alldatasheet.com" || strPath=="pdf1.alldatasheet.com" || strPath=="components.alldatasheet.com") strLang = "alldatasheet.com";


else if(strPath=="alldatasheet.net" || strPath=="www.alldatasheet.net" || strPath=="distributor.alldatasheet.net" || strPath=="manufacture.alldatasheet.net" || strPath=="html.alldatasheet.net" || strPath=="pdf1.alldatasheet.net" || strPath=="components.alldatasheet.net") strLang = "alldatasheet.net";
else if(strPath=="alldatasheetcn.com" || strPath=="www.alldatasheetcn.com" || strPath=="distributor.alldatasheetcn.com" || strPath=="manufacture.alldatasheetcn.com" || strPath=="html.alldatasheetcn.com" || strPath=="pdf1.alldatasheetcn.com" || strPath=="components.alldatasheetcn.com") strLang = "alldatasheetcn.com";
else if(strPath=="alldatasheetde.com" || strPath=="www.alldatasheetde.com" || strPath=="distributor.alldatasheetde.com" || strPath=="manufacture.alldatasheetde.com" || strPath=="html.alldatasheetde.com" || strPath=="pdf1.alldatasheetde.com" || strPath=="components.alldatasheetde.com") strLang = "alldatasheetde.com";
else if(strPath=="alldatasheet.jp" || strPath=="www.alldatasheet.jp" || strPath=="distributor.alldatasheet.jp" || strPath=="manufacture.alldatasheet.jp" || strPath=="html.alldatasheet.jp" || strPath=="pdf1.alldatasheet.jp" || strPath=="components.alldatasheet.jp") strLang = "alldatasheet.jp";
else if(strPath=="alldatasheetru.com" || strPath=="www.alldatasheetru.com" || strPath=="distributor.alldatasheetru.com" || strPath=="manufacture.alldatasheetru.com" || strPath=="html.alldatasheetru.com" || strPath=="pdf1.alldatasheetru.com" || strPath=="components.alldatasheetru.com") strLang = "alldatasheetru.com";
else if(strPath=="alldatasheet.co.kr" || strPath=="www.alldatasheet.co.kr" || strPath=="distributor.alldatasheet.co.kr" || strPath=="manufacture.alldatasheet.co.kr" || strPath=="html.alldatasheet.co.kr" || strPath=="pdf1.alldatasheet.co.kr" || strPath=="components.alldatasheet.co.kr") strLang = "alldatasheet.co.kr";
else if(strPath=="alldatasheet.es" || strPath=="www.alldatasheet.es" || strPath=="distributor.alldatasheet.es" || strPath=="manufacture.alldatasheet.es" || strPath=="html.alldatasheet.es" || strPath=="pdf1.alldatasheet.es" || strPath=="components.alldatasheet.es") strLang = "alldatasheet.es";
else if(strPath=="alldatasheet.fr" || strPath=="www.alldatasheet.fr" || strPath=="distributor.alldatasheet.fr" || strPath=="manufacture.alldatasheet.fr" || strPath=="html.alldatasheet.fr" || strPath=="pdf1.alldatasheet.fr" || strPath=="components.alldatasheet.fr") strLang = "alldatasheet.fr";
else if(strPath=="alldatasheetit.com" || strPath=="www.alldatasheetit.com" || strPath=="distributor.alldatasheetit.com" || strPath=="manufacture.alldatasheetit.com" || strPath=="html.alldatasheetit.com" || strPath=="pdf1.alldatasheetit.com" || strPath=="components.alldatasheetit.com") strLang = "alldatasheetit.com";
else if(strPath=="alldatasheetpt.com" || strPath=="www.alldatasheetpt.com" || strPath=="distributor.alldatasheetpt.com" || strPath=="manufacture.alldatasheetpt.com" || strPath=="html.alldatasheetpt.com" || strPath=="pdf1.alldatasheetpt.com" || strPath=="components.alldatasheetpt.com") strLang = "alldatasheetpt.com";
else if(strPath=="alldatasheet.pl" || strPath=="www.alldatasheet.pl" || strPath=="distributor.alldatasheet.pl" || strPath=="manufacture.alldatasheet.pl" || strPath=="html.alldatasheet.pl" || strPath=="pdf1.alldatasheet.pl" || strPath=="components.alldatasheet.pl") strLang = "alldatasheet.pl";
else if(strPath=="alldatasheet.vn" || strPath=="www.alldatasheet.vn" || strPath=="distributor.alldatasheet.vn" || strPath=="manufacture.alldatasheet.vn" || strPath=="html.alldatasheet.vn" || strPath=="pdf1.alldatasheet.vn" || strPath=="components.alldatasheet.vn") strLang = "alldatasheet.vn";
else if(strPath=="alldatasheet.in" || strPath=="www.alldatasheet.in") strLang = "alldatasheet.in";
else if(strPath=="alldatasheet.co.uk" || strPath=="www.alldatasheet.co.uk") strLang = "alldatasheet.co.uk";
else if(strPath=="alldatasheet.co.nz" || strPath=="www.alldatasheet.co.nz") strLang = "alldatasheet.co.nz";
else if(strPath=="alldatasheet.com.mx" || strPath=="www.alldatasheet.com.mx") strLang = "alldatasheet.com.mx";



function showf(){
	document.getElementById('idflag').style.left=document.body.clientWidth-278;
	if(document.getElementById('idflag').style.visibility=='visible'){
		document.getElementById('idflag').style.visibility='hidden';
		document.getElementById('idflag').style.display='none';
	}else{
		document.getElementById('idflag').style.visibility='visible';
		document.getElementById('idflag').style.display='block';
	}
}
function search_popup(el,le,to){
    var $el = $(el);        //레이어의 id를 $el 변수에 저장
    var isDim = $el.prev().hasClass('searchBg');   //dimmed 레이어를 감지하기 위한 boolean 변수
    isDim ? $('.dim-layer-s').fadeIn() : $el.fadeIn();
    var $elWidth = ~~($el.outerWidth()),
        $elHeight = ~~($el.outerHeight()),
        docWidth = $(document).width(),
        docHeight = $(document).height();
    // 화면의 중앙에 레이어를 띄운다.
//    if ($elHeight < docHeight || $elWidth < docWidth) {
        if (window.innerWidth >= 700) {
        $el.css({
//            marginTop: -$elHeight /2,
//            marginLeft: -$elWidth/2

          left: le,
          top: to+30


        })
    } else {
        $el.css({
        	left: 0,
            top: to+30
       	})
//        $el.css('width','98%');
    }

    $('.searchBg').click(function(){
        $('.dim-layer-s').fadeOut();
        return false;
    });
    jQuery( window ).resize(function() {  
        if (window.innerWidth >= 700) {
            $el.css({
            	left: window.innerWidth/2-214,
	            top: to+30
            })
        } else {
            $el.css({
            	left: 0,
	            top: to+90
           	})
        }
    });
}
$('.btn-example').click(function(){
	var v_thisOffset = $(this).offset();
    layer_popup("#layer",v_thisOffset.left,v_thisOffset.top);
    mfg_fill();
});
function layer_popup(el,le,to){
	
	$('#layer_sSearchword').val($('#sSearchword').val());
	$('#layer_sField').val(frmSearch.sField.value);
	
    var $el = $(el);        //레이어의 id를 $el 변수에 저장
    var isDim = $el.prev().hasClass('dimBg');   //dimmed 레이어를 감지하기 위한 boolean 변수
    isDim ? $('.dim-layer').fadeIn() : $el.fadeIn();
    var $elWidth = ~~($el.outerWidth()),
        $elHeight = ~~($el.outerHeight()),
        docWidth = $(document).width(),
        docHeight = $(document).height();
    // 화면의 중앙에 레이어를 띄운다.
//    if ($elHeight < docHeight || $elWidth < docWidth) {
        if (window.innerWidth >= 700) {
        $el.css({
//            marginTop: -$elHeight /2,
//            marginLeft: -$elWidth/2

          left: le-520,
          top: to+70


        })
    } else {
        $el.css({
        	left: 0,
            top: to+70
       	})
        $el.css('width','98%');
        
        $('#layer_sSearchword').css('width','98%');
        $('#select_mfg').css('width','98%');
    }
    $el.find('a.btn-layerClose').click(function(){
        isDim ? $('.dim-layer').fadeOut() : $el.fadeOut(); // 닫기 버튼을 클릭하면 레이어가 닫힌다.
        
        frmSearch.sField.value = $('#layer_sField').val();
        frmSearch.sSearchword.value = $('#layer_sSearchword').val();
        if($("#select_mfg option:selected").val()!=0){
        	$("<input></input>").attr({type:"hidden", name:"list", value:""}).appendTo($("[name=frmSearch]"));
        	frmSearch.list.value = $("#select_mfg option:selected").val();
        }

        if(InputCheck(frmSearch,$(":input:radio[name=radio_search]:checked").val())) frmSearch.submit();
        return false;
    });
    $('.dimBg').click(function(){
        frmSearch.sField.value = $('#layer_sField').val();
        frmSearch.sSearchword.value = $('#layer_sSearchword').val();
        $('.dim-layer').fadeOut();
        return false;
    });
    $('.close').click(function(){
        frmSearch.sField.value = $('#layer_sField').val();
        frmSearch.sSearchword.value = $('#layer_sSearchword').val();
        $('.dim-layer').fadeOut();
        return false;
    });

    
    $(":input:radio[name=radio_search]").change(function(){
	    if($(":input:radio[name=radio_search]:checked").val()==2) {
	    	$("#layer_sField").val("0");
	    	$("#layer_sField option").not(":selected").attr("disabled", "disabled");
	    } else { 
	    	$("#layer_sField option").not(":selected").removeAttr("disabled");
	    }
    });

    jQuery( window ).resize(function() {
        if (window.innerWidth >= 700) {
            $el.css({
            	left: window.innerWidth/2-360,
	            top: to+70,
	            width: 700
            })
            $('#layer_sSearchword').css('width','455');
            $('#select_mfg').css('width','560');
        } else {
            $el.css({
            	left: 0
           	})
            $el.css('width','98%');
            
            $('#layer_sSearchword').css('width','98%');
            $('#select_mfg').css('width','98%');
            
        }
    });
}
$('#frmMore').click(function(){
	if($('#tableMore').css("display") != "none"){
		$('#tableMore').css("display","none");
	} else {
		$('#tableMore').css("display","block");
	}
    mfg_fill();
});
$('#frmSubmit').click(function(){
    frmSearch.sField.value = $('#layer_sField').val();
    if($("#select_mfg option:selected").val()!=0){
    	$("<input></input>").attr({type:"hidden", name:"list", value:""}).appendTo($("[name=frmSearch]"));
    	frmSearch.list.value = $("#select_mfg option:selected").val();
    }
    if(InputCheck(frmSearch, document.getElementById('sSort').options[document.getElementById('sSort').selectedIndex].value)) frmSearch.submit();
    return false;
});
$("#sSort").change(function(){
    if($("#sSort option:selected").val()==2) {
    	$("#layer_sField").val("0");
    	$("#layer_sField option").not(":selected").attr("disabled", "disabled");
    } else { 
    	$("#layer_sField option").not(":selected").removeAttr("disabled");
    }
});
function chageLangSelect(){
    var langSelect = document.getElementById("sSort");
    var selectValue = langSelect.options[langSelect.selectedIndex].value;
    var selectText = langSelect.options[langSelect.selectedIndex].text;
    var stringPlace = "";

if(strLang == "alldatasheetcn.com") {
	if(selectText == "型号" && (selectValue == "1" || selectValue == "11" || selectValue == "13")) stringPlace = "请输入型号";
	if(selectText == "描述" && selectValue == "2") stringPlace = "请输入描述";
	if(selectText == "标识" && selectValue == "3") stringPlace = "请输入标识";
} else if(strLang == "alldatasheet.co.kr") {
	if(selectText == "부품명" &&  (selectValue == "1" || selectValue == "11" || selectValue == "13")) stringPlace = "부품명을 입력하세요";
	if(selectText == "상세내용" && selectValue == "2") stringPlace = "상세내용을 입력하세요";
	if(selectText == "마킹넘버" && selectValue == "3") stringPlace = "마킹넘버를 입력하세요";
} else {
    if(selectText == "Part No." && (selectValue == "1" || selectValue == "11" || selectValue == "13")) stringPlace = "Enter a Part Number";
	if(selectText == "Description" && selectValue == "2") stringPlace = "Enter a Description";
	if(selectText == "Marking" && selectValue == "3") stringPlace = "Enter a Marking";
}
	
	document.getElementById('sSearchword').placeholder = stringPlace; 
}
