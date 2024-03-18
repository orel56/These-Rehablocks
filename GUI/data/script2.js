console.log("bonjour");

function my_periodic_method(){
    console.log("periodicity");

$.ajax({
    url: "/data",
    success: display_data,
    complete: periodic_call,
    dataType: "json"
});
}
        
function display_data(result) {
    document.getElementById("device").innerHTML = result;
}

function periodic_call(){
   setTimeout(my_periodic_method, 1000);
}

setTimeout(my_periodic_method, 1000);