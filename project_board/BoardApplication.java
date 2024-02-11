package main.borad;

import java.util.List;
import java.util.Scanner;

public class BoardApplication {
    public static void main(String[] args) {
        String start = "[ showAll, create, read, update, delete, exit ]";
        System.out.println(start);

        Scanner sc = new Scanner(System.in);

        BulletinBoard board = new BulletinBoard();

        while(true){
            String input = sc.next();
            if(input.equals("showAll")){
                List<Article> list = board.showAll();
                for(int i=0; i<list.size(); i++){
                    Article article = list.get(i);
                    System.out.println("ArticleNum: " + article.getArticleNum() + "  UserId: " + article.getUserId()
                            + "  Title: " + article.getTitle() + "  Content: " + article.getContent());}
            }
            else if(input.equals("create")){
                System.out.print("UserId: ");
                String userId = sc.next();
                System.out.print("Title: ");
                String title = sc.next();
                System.out.print("Content: ");
                String content = sc.next();
                board.create(userId, title, content);
            }
            else if(input.equals("read")){
                System.out.print("ArticleNum: ");
                int Num = sc.nextInt();
                Article article = board.read(Num);
                System.out.println("ArticleNum: " + article.getArticleNum() + "  UserId: " + article.getUserId()
                        + "  Title: " + article.getTitle() + "  Content: " + article.getContent());
            }
            else if(input.equals("update")){
                System.out.print("ArticleNum: ");
                int articleNum = sc.nextInt();
                System.out.print("Title: ");
                String title = sc.next();
                System.out.print("Content: ");
                String content = sc.next();
                board.update(articleNum, title, content);
            }
            else if(input.equals("delete")){
                System.out.print("ArticleNum: ");
                int articleNum = sc.nextInt();
                board.delete(articleNum);
            }
            else if(input.equals("exit")){
                System.exit(0);
            }
        }

    }
}
