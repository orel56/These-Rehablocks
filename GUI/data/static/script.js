console.log("bonjour");

function my_periodic_method(){console.log("Au revoir");
}
$.ajax({
    url: "/data",
    success: display_data,
    complete: periodic_call});

        
function display_data(result) {
    console.log("Résultat de la requête :", result);

}
function periodic_call(){
    console.log("Au revoir");
}

setTimeout(my_periodic_method, 1000);