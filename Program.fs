open FSharp.Data
open System.IO
open System.Net
open System.Text

open System.Text.RegularExpressions
open System.IO
open System.Net

let mutable films_cnt = 0
let films : Map<string, int> = Map.empty

let main () =
  let users = ["5983110";"5994802";"5989381";"5994916";"3849416";"4602140";"3992200";"5958127";"3107821";"972746";"4308134";"312992";"5989381";"4156008";"2306495";"1162080";"2294940";"3663196";"3280724";"3175803"]
  let (films, films_lst) = 
   users 
    |> List.fold (fun (films,list) user_id ->
        let packages = HtmlDocument.Load("http://www.kinopoisk.ru/user/" + user_id + "/votes/")
        packages.Descendants ["div"] 
            |> Seq.filter (fun x -> x.HasClass("item") || x.HasClass("item even"))
            |> Seq.collect (fun (x:HtmlNode) -> x.Descendants ["a"])
            |> Seq.map (fun x -> x.InnerText())
            |> Seq.toList
            |> List.fold (fun (films,list) name -> printfn "%s %i" name films_cnt
                                                   if not (Map.containsKey name films)
                                                   then films_cnt <- films_cnt+1
                                                        (Map.add name films_cnt films,name::list)
                                                   else (films, name::list)
                      ) (films,list)
        ) (Map.empty,[])
  let films_list = List.rev films_lst
  Map.iter (fun k v -> printfn "%s %i" k v) films

  let votes =
   users
    |> List.fold (fun (l:string list) user_id ->
        let packages = HtmlDocument.Load("http://www.kinopoisk.ru/user/" + user_id + "/votes/")
        let l1 = 
           packages.Descendants ["div"] 
            |> Seq.filter (fun x -> x.HasClass("vote"))
            |> Seq.choose (fun (x:HtmlNode) -> if x.InnerText().Length < 3 && x.InnerText().Length > 0
                                               then Some(x.InnerText()) 
                                               else if x.InnerText().Length = 0 
                                                    then Some("0")
                                                    else None)
            |> Seq.toList
        l@l1
        ) []

  let user_film_vote = List.zip films_list votes
  List.iteri (fun i x -> printfn "%i %s" i x) votes
  let write_file l =
    use sw = new StreamWriter("/Users/luba_yaronskaya/Documents/kinopoisk_res.txt")
    let users_cnt = List.length users
    sw.WriteLine(users_cnt.ToString() + " " + films_cnt.ToString() + " " + (List.length votes).ToString() + " 5")
    List.iteri (fun i (f_name, vote) ->
                                           let user = i / 50
                                           let film_id = Map.find f_name films
                                           sw.WriteLine(user.ToString()+" "+film_id.ToString()+" "+vote)
                  ) l
    sw.Close()

  write_file user_film_vote
